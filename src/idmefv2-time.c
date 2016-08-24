/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@libidmefv2-ids.com>
*
* This file is part of the LibIdemfv2 library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

/*
 * This is required on Solaris so that multiple call to
 * strptime() won't reset the tm structure.
 */
#define _STRPTIME_DONTZERO

#include "config.h"
#include "libmissing.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#if defined(__linux__) && ! defined(__USE_XOPEN)
# define __USE_XOPEN
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include "libidmefv2-inttypes.h"
#include "libidmefv2-string.h"
#include "libidmefv2-error.h"
#include "ntp.h"
#include "libidmefv2-log.h"
#include "common.h"
#include "idmefv2-time.h"


static char *parse_time_ymd(struct tm *tm, const char *buf)
{
        char *ptr;

        ptr = strptime(buf, "%Y-%m-%d", tm);
        if ( ! ptr )
                return NULL;

        /*
         * The IDMEFV2 draft only permits the 'T' variant here
         */
        while ( isspace((int) *ptr) )
                ptr++;

        if ( *ptr == 'T' )
                ptr++;

        return ptr;
}



static int digit2usec(uint32_t n, int digit_count)
{
        int i;
        const size_t max_digit = 6; /* 999999 */

        if ( digit_count > max_digit )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "Invalid number of digits for time fraction");

        for ( i = 0; i < (max_digit - digit_count); i++ )
                n *= 10;

        return n;
}



static int parse_time_hmsu(struct tm *tm, uint32_t *usec, char **buf)
{
        int fraction;
        char *eptr = NULL;

        *buf = strptime(*buf, "%H:%M:%S", tm);
        if ( ! *buf )
                goto fmterror;

        if ( **buf == '.' || **buf == ',' ) {
                (*buf)++;

                fraction = strtoul(*buf, &eptr, 10);
                if ( eptr == *buf )
                        goto fmterror;

                fraction = digit2usec(fraction, eptr - *buf);
                if ( fraction < 0 )
                        return fraction;

                *buf = eptr;
                *usec = fraction;
        }

        return 0;

    fmterror:
        return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "error parsing time field, format should be: HH:MM:SS(.fraction)");
}



static int parse_time_gmt(struct tm *tm, int32_t *gmtoff, const char *buf)
{
        int ret;
        unsigned int offset_hour, offset_min;

        /*
         * if UTC, do nothing.
         */
        if ( *buf == 'Z' )
                return 0;

        ret = sscanf(buf + 1, "%2u:%2u", &offset_hour, &offset_min);
        if ( ret != 2 )
                return -1;

        if ( *buf == '+' ) {
                tm->tm_min -= offset_min;
                tm->tm_hour -= offset_hour;
                *gmtoff = offset_hour * 3600 + offset_min * 60;
        }

        else if ( *buf == '-' ) {
                tm->tm_min += offset_min;
                tm->tm_hour += offset_hour;
                *gmtoff = - (offset_hour * 3600 + offset_min * 60);
        }

        else
                return -1;

        return 0;
}



/**
 * idmefv2_time_set_from_string:
 * @time: Pointer to an #idmefv2_time_t object.
 * @buf: Pointer to a string describing a time in an IDMEFV2 conforming format.
 *
 * Fills @time object with information retrieved from the user provided
 * @buf, containing a string describing a time in a format conforming
 * to the IDMEFV2 definition (v. 0.10, section 3.2.6).
 *
 * Additionally, the provided time might be separated with white spaces,
 * instead of the IDMEFV2 defined 'T' character.
 *
 * If there is no UTC offset specified, we assume that the provided
 * time is local, and compute the GMT offset by ourselve.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_set_from_string(idmefv2_time_t *time, const char *buf)
{
        int ret;
        char *ptr;
        struct tm tm;
        libidmefv2_bool_t miss_gmt = FALSE;

        libidmefv2_return_val_if_fail(time, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        memset(&tm, 0, sizeof(tm));
        tm.tm_isdst = -1;

        ptr = parse_time_ymd(&tm, buf);
        if ( ! ptr )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "error parsing date field, format should be: YY-MM-DD");

        time->usec = 0;
        time->gmt_offset = 0;

        if ( *ptr ) {
                ret = parse_time_hmsu(&tm, &time->usec, &ptr);
                if ( ret < 0 )
                        return ret;

                miss_gmt = TRUE;
                if ( *ptr ) {
                        ret = parse_time_gmt(&tm, &time->gmt_offset, ptr);
                        if ( ret < 0 )
                                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "error parsing GMT offset field (Z)?(+|-)?HH:MM");

                        miss_gmt = FALSE;
                }
        }

        if ( miss_gmt ) {
                long gmtoff;
                libidmefv2_get_gmt_offset_from_tm(&tm, &gmtoff);
                time->gmt_offset = (int32_t) gmtoff;
        }

        time->sec = miss_gmt ? mktime(&tm) : libidmefv2_timegm(&tm);
        return 0;
}



/**
 * idmefv2_time_new_from_string:
 * @time: Address where to store the created #idmefv2_time_t object.
 * @buf: Pointer to a string describing a time in an IDMEFV2 conforming format.
 *
 * Creates an #idmefv2_time_t object filled with information retrieved
 * from the user provided @buf, containing a string describing a time in a format
 * conforming to the IDMEFV2 definition  (v. 0.10, section 3.2.6).
 *
 * Additionally, the provided time might be separated with white spaces, instead
 * of the IDMEFV2 define 'T' character. The format might not specify a timezone
 * (will assume UTC in this case).
 *
 * The resulting #idmefv2_time_t object is stored in @time.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_new_from_string(idmefv2_time_t **time, const char *buf)
{
        int ret;

        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_time_new(time);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_time_set_from_string(*time, buf);
        if ( ret < 0 ) {
                free(*time);
                return ret;
        }

        return 0;
}



/**
 * idmefv2_time_set_from_ntpstamp:
 * @time: Pointer to a #idmefv2_time_t object.
 * @buf: Pointer to a string containing an NTP timestamp.
 *
 * Fills the @time object with information provided within the @buf NTP timestamp.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_set_from_ntpstamp(idmefv2_time_t *time, const char *buf)
{
        l_fp ts;
        struct timeval tv;
        unsigned ts_mask = TS_MASK;
        unsigned ts_roundbit = TS_ROUNDBIT;

        libidmefv2_return_val_if_fail(time, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( sscanf(buf, "%x.%x", &ts.l_ui, &ts.l_uf) < 2 )
                return -1;

        /*
         * This transformation is a reverse form of the one found in
         *  idmefv2_get_ntp_timestamp()
         */
        ts.l_ui -= JAN_1970;
        ts.l_uf -= ts_roundbit;
        ts.l_uf &= ts_mask;
        TSTOTV(&ts, &tv);

        time->sec = tv.tv_sec;
        time->usec = tv.tv_usec;
        time->gmt_offset = 0;

        return 0;
}



/**
 * idmefv2_time_to_ntpstamp:
 * @time: Pointer to an IDMEFV2 time structure.
 * @out: Pointer to a #libidmefv2_string_t output buffer.
 *
 * Translates @time to an user readable NTP timestamp string,
 * conforming to the IDMEFV2 defined time format.
 *
 * Returns: number of bytes written on success, a negative value if an error occured.
 */
int idmefv2_time_to_ntpstamp(const idmefv2_time_t *time, libidmefv2_string_t *out)
{
        l_fp ts;
        struct timeval tv;
        unsigned ts_mask = TS_MASK;             /* defaults to 20 bits (us) */
        unsigned ts_roundbit = TS_ROUNDBIT;     /* defaults to 20 bits (us) */
        int ret;

        libidmefv2_return_val_if_fail(time, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(out, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        tv.tv_sec = idmefv2_time_get_sec(time);
        tv.tv_usec = idmefv2_time_get_usec(time);

        sTVTOTS(&tv, &ts);

        ts.l_ui += JAN_1970;                    /* make it time since 1900 */
        ts.l_uf += ts_roundbit;
        ts.l_uf &= ts_mask;

        ret = libidmefv2_string_sprintf(out, "0x%08lx.0x%08lx", (unsigned long) ts.l_ui, (unsigned long) ts.l_uf);

        return ret;
}



/**
 * idmefv2_time_to_string:
 * @time: Pointer to an IDMEFV2 time structure.
 * @out: Pointer to a #libidmefv2_string_t output buffer.
 *
 * Translates @time to an user readable string conforming to the IDMEFV2
 * defined time format.
 *
 * Returns: number of bytes written on success, a negative value if an error occured.
 */
int idmefv2_time_to_string(const idmefv2_time_t *time, libidmefv2_string_t *out)
{
        time_t t;
        struct tm utc;
        uint32_t hour_off, min_off;

        libidmefv2_return_val_if_fail(time, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(out, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        t = time->sec + time->gmt_offset;

        if ( ! gmtime_r((const time_t *) &t, &utc) )
                return libidmefv2_error_from_errno(errno);

        hour_off = time->gmt_offset / 3600;
        min_off = time->gmt_offset % 3600 / 60;

        return libidmefv2_string_sprintf(out, "%d-%.2d-%.2dT%.2d:%.2d:%.2d.%02u%+.2d:%.2d",
                                      utc.tm_year + 1900, utc.tm_mon + 1, utc.tm_mday,
                                      utc.tm_hour, utc.tm_min, utc.tm_sec, idmefv2_time_get_usec(time),
                                      hour_off, min_off);
}




/**
 * idmefv2_time_new_from_ntpstamp:
 * @time: Address where to store the created #idmefv2_time_t object.
 * @buf: Pointer to a string containing an NTP timestamp.
 *
 * Creates an #idmefv2_time_t object filled with information provided
 * from the @buf NTP timestamp, and stores it in @time.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_new_from_ntpstamp(idmefv2_time_t **time, const char *buf)
{
        int ret;

        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_time_new(time);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_time_set_from_ntpstamp(*time, buf);
        if ( ret < 0 ) {
                free(*time);
                return ret;
        }

        return 0;
}



/**
 * idmefv2_time_set_from_timeval:
 * @time: Pointer to an #idmefv2_time_t object.
 * @tv: Pointer to a struct timeval (see gettimeofday()).
 *
 * Fills @time object filled with information provided within the @tv structure.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_set_from_timeval(idmefv2_time_t *time, const struct timeval *tv)
{
        int ret;
        long gmtoff;

        libidmefv2_return_val_if_fail(time, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(tv, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = libidmefv2_get_gmt_offset_from_time((const time_t *) &tv->tv_sec, &gmtoff);
        if ( ret < 0 )
                return ret;

        time->sec = tv->tv_sec;
        time->usec = tv->tv_usec;
        time->gmt_offset = (int32_t) gmtoff;

        return 0;
}



/**
 * idmefv2_time_new_from_timeval:
 * @time: Address where to store the created #idmefv2_time_t object.
 * @tv: Pointer to a struct timeval (see gettimeofday()).
 *
 * Creates an #idmefv2_time_t object filled with information provided
 * within the @tv structure.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_new_from_timeval(idmefv2_time_t **time, const struct timeval *tv)
{
        int ret;

        libidmefv2_return_val_if_fail(tv, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_time_new(time);
        if ( ret < 0 )
                return ret;

        return idmefv2_time_set_from_timeval(*time, tv);
}



/**
 * idmefv2_time_set_from_gettimeofday:
 * @time: Pointer to an #idmefv2_time_t object.
 *
 * Fills @time with information retrieved using gettimeofday().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_set_from_gettimeofday(idmefv2_time_t *time)
{
        int ret;
        struct timeval tv;

        libidmefv2_return_val_if_fail(time, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = gettimeofday(&tv, NULL);
        if ( ret < 0 )
                return libidmefv2_error_from_errno(errno);

        return idmefv2_time_set_from_timeval(time, &tv);
}



/**
 * idmefv2_time_new_from_gettimeofday:
 * @time: Address where to store the created #idmefv2_time_t object.
 *
 * Creates an #idmefv2_time_t object filled with information retrieved
 * using gettimeofday(), and stores it in @time.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_new_from_gettimeofday(idmefv2_time_t **time)
{
        int ret;
        struct timeval tv;

        ret = gettimeofday(&tv, NULL);
        if ( ret < 0 )
                return libidmefv2_error_from_errno(errno);

        return idmefv2_time_new_from_timeval(time, &tv);
}



/**
 * idmefv2_time_ref:
 * @time: Pointer to an #idmefv2_time_t object.
 *
 * Increases @time reference count.
 * idmefv2_time_destroy() won't destroy @time until the refcount
 * reach 0.
 *
 * Returns: The @time provided argument.
 */
idmefv2_time_t *idmefv2_time_ref(idmefv2_time_t *time)
{
        libidmefv2_return_val_if_fail(time, NULL);

        time->refcount++;
        return time;
}



/**
 * idmefv2_time_new:
 * @time: Address where to store the created #idmefv2_time_t object.
 *
 * Creates an empty #idmefv2_time_t object and store it in @time.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_new(idmefv2_time_t **time)
{
        *time = calloc(1, sizeof(**time));
        if ( ! *time )
                return libidmefv2_error_from_errno(errno);

        (*time)->refcount = 1;

        return 0;
}



/**
 * idmefv2_time_clone:
 * @src: Pointer to a #idmefv2_time_t to clone.
 * @dst: Address where to store the cloned @src object.
 *
 * Clones @src and stores the result in the @dst address.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_clone(const idmefv2_time_t *src, idmefv2_time_t **dst)
{
        int ret;

        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_time_new(dst);
        if ( ret < 0 )
                return ret;

        return idmefv2_time_copy(src, *dst);
}



/**
 * idmefv2_time_set_from_time:
 * @time: Pointer to an #idmefv2_time_t object.
 * @t: Pointer to a time_t.
 *
 * Fills @time from the information described by @t.
 * @time won't contain micro seconds information, since theses are not
 * available within @t.
 */
void idmefv2_time_set_from_time(idmefv2_time_t *time, const time_t *t)
{
        long gmtoff;

        libidmefv2_return_if_fail(time);
        libidmefv2_return_if_fail(t);

        libidmefv2_get_gmt_offset_from_time(t, &gmtoff);

        time->gmt_offset = (int32_t) gmtoff;
        time->sec = *t;
}



/**
 * idmefv2_time_new_from_time:
 * @time: Address where to store the created #idmefv2_time_t object.
 * @t: Pointer to a time_t.
 *
 * Creates a new #idmefv2_time_t object and store it in @time.
 * This object will be filled with information available in @t. The created
 * @time won't contain micro seconds information, since theses are not
 * available within @t.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_new_from_time(idmefv2_time_t **time, const time_t *t)
{
        int ret;

        libidmefv2_return_val_if_fail(t, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_time_new(time);
        if ( ret < 0 )
                return ret;

        idmefv2_time_set_from_time(*time, t);

        return 0;
}



/**
 * idmefv2_time_set_gmt_offset:
 * @time: Pointer to a #idmefv2_time_t.
 * @gmtoff: GMT offset for @time, in seconds.
 *
 * Sets the GMT offset @gmtoff, in seconds, within @time.
 *
 * WARNING: this is just an accessor function, and using it to
 * set @time current time also requires the use of idmefv2_time_set_sec()
 * and idmefv2_time_set_usec().
 */
void idmefv2_time_set_gmt_offset(idmefv2_time_t *time, int32_t gmtoff)
{
        libidmefv2_return_if_fail(time);
        time->gmt_offset = gmtoff;
}



/**
 * idmefv2_time_set_sec:
 * @time: Pointer to a #idmefv2_time_t.
 * @sec: Number of seconds since the Epoch.
 *
 * Sets the number of second from the Epoch to @sec within @time.
 *
 * WARNING: this is just an accessor function, and using it to
 * set @time current time also requires the use of idmefv2_time_set_usec()
 * and idmefv2_time_set_gmt_offset().
 */
void idmefv2_time_set_sec(idmefv2_time_t *time, uint32_t sec)
{
        libidmefv2_return_if_fail(time);
        time->sec = sec;
}



/**
 * idmefv2_time_set_usec:
 * @time: Pointer to a #idmefv2_time_t.
 * @usec: Number of micro seconds to set within @time.
 *
 * Sets the number of micro second to @usec within @time.
 *
 * WARNING: this is just an accessor function, and using it to
 * set @time current time also requires the use of idmefv2_time_set_sec()
 * and idmefv2_time_set_gmt_offset().
 */
void idmefv2_time_set_usec(idmefv2_time_t *time, uint32_t usec)
{
        libidmefv2_return_if_fail(time);
        time->usec = usec;
}


/**
 * idmefv2_time_get_gmt_offset:
 * @time: Pointer to a #idmefv2_time_t.
 *
 * Returns the GMT offset that applies to @time.
 *
 * Returns: The GMT offset, in seconds.
 */
int32_t idmefv2_time_get_gmt_offset(const idmefv2_time_t *time)
{
        libidmefv2_return_val_if_fail(time, 0);
        return time->gmt_offset;
}




/**
 * idmefv2_time_get_sec:
 * @time: Pointer to a #idmefv2_time_t.
 *
 * Returns the number of second since the Epoch (00:00:00 UTC, January 1, 1970),
 * previously set within @time.
 *
 * Returns: The number of seconds.
 */
uint32_t idmefv2_time_get_sec(const idmefv2_time_t *time)
{
        libidmefv2_return_val_if_fail(time, 0);
        return time->sec;
}



/**
 * idmefv2_time_get_usec:
 * @time: Pointer to a #idmefv2_time_t.
 *
 * Returns the u-second member of @time.
 *
 * Returns: The number of u-seconds.
 */
uint32_t idmefv2_time_get_usec(const idmefv2_time_t *time)
{
        libidmefv2_return_val_if_fail(time, 0);
        return time->usec;
}



/**
 * idmefv2_time_copy:
 * @src: Pointer to a #idmefv2_time_t to copy data from.
 * @dst: Pointer to a #idmefv2_time_t to copy data to.
 *
 * Copies @src internal to @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_time_copy(const idmefv2_time_t *src, idmefv2_time_t *dst)
{
        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(dst, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        dst->sec = src->sec;
        dst->usec = src->usec;
        dst->gmt_offset = src->gmt_offset;

        return 0;
}




void idmefv2_time_destroy_internal(idmefv2_time_t *time)
{
        /* nop */
}



/**
 * idmefv2_time_destroy:
 * @time: Pointer to an #idmefv2_time_t object.
 *
 * Destroys @time if refcount reach 0.
 */
void idmefv2_time_destroy(idmefv2_time_t *time)
{
        libidmefv2_return_if_fail(time);

        if ( --time->refcount )
                return;

        free(time);
}



/**
 * idmefv2_time_compare:
 * @time1: Pointer to an #idmefv2_time_t object to compare with @time2.
 * @time2: Pointer to an #idmefv2_time_t object to compare with @time1.
 *
 * Returns: 0 if @time1 and @time2 match, 1 if @time1 is greater than
 * @time2, -1 if @time1 is lesser than @time2.
 */
int idmefv2_time_compare(const idmefv2_time_t *time1, const idmefv2_time_t *time2)
{
        unsigned long t1, t2;

        if ( ! time1 && ! time2 )
                return 0;

        else if ( ! time1 || ! time2 )
                return -1;

        t1 = time1->sec + time1->gmt_offset;
        t2 = time2->sec + time2->gmt_offset;

        if ( t1 == t2 ) {
                if ( time1->usec == time2->usec )
                        return 0;
                else
                        return (time1->usec < time2->usec) ? -1 : 1;
        }

        else return (t1 < t2) ? -1 : 1;
}

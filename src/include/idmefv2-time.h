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

#ifndef _LIBIDMEFV2_IDMEFV2_TIME_H
#define _LIBIDMEFV2_IDMEFV2_TIME_H

#ifdef HAVE_CONFIG_H
# include "config.h"
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

#ifdef __cplusplus
 extern "C" {
#endif

struct idmefv2_time {
        /* <private> */
        int refcount;
        uint32_t sec;
        uint32_t usec;
        int32_t gmt_offset;
};

typedef struct idmefv2_time idmefv2_time_t;

idmefv2_time_t *idmefv2_time_ref(idmefv2_time_t *time);
int idmefv2_time_new(idmefv2_time_t **time);

int idmefv2_time_new_from_time(idmefv2_time_t **time, const time_t *t);
int idmefv2_time_new_from_gettimeofday(idmefv2_time_t **time);
int idmefv2_time_new_from_string(idmefv2_time_t **time, const char *buf);
int idmefv2_time_new_from_ntpstamp(idmefv2_time_t **time, const char *buf);
int idmefv2_time_new_from_timeval(idmefv2_time_t **time, const struct timeval *tv);

void idmefv2_time_set_from_time(idmefv2_time_t *time, const time_t *t);
int idmefv2_time_set_from_gettimeofday(idmefv2_time_t *time);
int idmefv2_time_set_from_string(idmefv2_time_t *time, const char *buf);
int idmefv2_time_set_from_ntpstamp(idmefv2_time_t *time, const char *buf);
int idmefv2_time_set_from_timeval(idmefv2_time_t *time, const struct timeval *tv);

void idmefv2_time_destroy_internal(idmefv2_time_t *time);
void idmefv2_time_destroy(idmefv2_time_t *time);

int idmefv2_time_clone(const idmefv2_time_t *src, idmefv2_time_t **dst);
int idmefv2_time_copy(const idmefv2_time_t *src, idmefv2_time_t *dst);

void idmefv2_time_set_sec(idmefv2_time_t *time, uint32_t sec);
void idmefv2_time_set_usec(idmefv2_time_t *time, uint32_t usec);
void idmefv2_time_set_gmt_offset(idmefv2_time_t *time, int32_t gmtoff);

uint32_t idmefv2_time_get_sec(const idmefv2_time_t *time);
uint32_t idmefv2_time_get_usec(const idmefv2_time_t *time);
int32_t idmefv2_time_get_gmt_offset(const idmefv2_time_t *time);

int idmefv2_time_to_string(const idmefv2_time_t *time, libidmefv2_string_t *out);
int idmefv2_time_to_ntpstamp(const idmefv2_time_t *time, libidmefv2_string_t *out);

int idmefv2_time_compare(const idmefv2_time_t *time1, const idmefv2_time_t *time2);
         
#ifdef __cplusplus
 }
#endif
         
#endif /* _LIBIDMEFV2_IDMEFV2_TIME_H */

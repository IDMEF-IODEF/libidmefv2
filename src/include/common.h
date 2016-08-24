/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libidmefv2-ids.com>
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

#ifndef _LIBIDMEFV2_COMMON_H
#define _LIBIDMEFV2_COMMON_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "idmefv2.h"
#include "libidmefv2-inttypes.h"
#include "libidmefv2-log.h"
#include <sys/types.h>

#ifdef WIN32
# include <winsock2.h>
#else
# include <sys/socket.h>
# include <netinet/in.h>
#endif

#include <time.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define libidmefv2_return_val_if_fail(cond, val) do {                               \
        if ( ! (cond) ) {                                                        \
                libidmefv2_log(LIBIDMEFV2_LOG_CRIT, "assertion '%s' failed\n", #cond); \
                return val;                                                      \
        }                                                                        \
} while(0)


#define libidmefv2_return_if_fail(cond) do {                                        \
        if ( ! (cond) ) {                                                        \
                libidmefv2_log(LIBIDMEFV2_LOG_CRIT, "assertion '%s' failed\n", #cond); \
                return;                                                          \
        }                                                                        \
} while(0)


int libidmefv2_parse_address(const char *str, char **addr, unsigned int *port);

uint64_t libidmefv2_hton64(uint64_t val);

uint32_t libidmefv2_htonf(float fval);

time_t libidmefv2_timegm(struct tm *tm);

int libidmefv2_get_gmt_offset(long *gmt_offset);

int libidmefv2_get_gmt_offset_from_tm(struct tm *tm, long *gmtoff);

int libidmefv2_get_gmt_offset_from_time(const time_t *utc, long *gmtoff);

int libidmefv2_read_multiline(FILE *fd, unsigned int *line, char *buf, size_t size);

int libidmefv2_read_multiline2(FILE *fd, unsigned int *line, libidmefv2_string_t *out);

void *libidmefv2_sockaddr_get_inaddr(struct sockaddr *sa);

void *_libidmefv2_realloc(void *ptr, size_t size);

int _libidmefv2_load_file(const char *filename, unsigned char **fdata, size_t *outsize);

void _libidmefv2_unload_file(unsigned char *fdata, size_t size);

double libidmefv2_simple_strtod(const char *s, char **endptr);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_COMMON_H */

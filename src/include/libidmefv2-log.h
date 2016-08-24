/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIDMEFV2_LIBIDMEFV2_LOG_H
#define _LIBIDMEFV2_LIBIDMEFV2_LOG_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libidmefv2-macros.h"
#include <stdarg.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        LIBIDMEFV2_LOG_CRIT  = -1,
        LIBIDMEFV2_LOG_ERR   =  0,
        LIBIDMEFV2_LOG_WARN  =  1,
        LIBIDMEFV2_LOG_INFO  =  2,
        LIBIDMEFV2_LOG_DEBUG  = 3
} libidmefv2_log_t;


typedef enum {
        LIBIDMEFV2_LOG_FLAGS_QUIET  = 0x01, /* Drop LIBIDMEFV2_LOG_PRIORITY_INFO */
        LIBIDMEFV2_LOG_FLAGS_SYSLOG = 0x02
} libidmefv2_log_flags_t;



void _libidmefv2_log_v(libidmefv2_log_t level, const char *file,
                    const char *function, int line, const char *fmt, va_list ap)
                    LIBIDMEFV2_FMT_PRINTF(5, 0);

void _libidmefv2_log(libidmefv2_log_t level, const char *file,
                  const char *function, int line, const char *fmt, ...)
                  LIBIDMEFV2_FMT_PRINTF(5, 6);


#ifdef HAVE_VARIADIC_MACROS

#define libidmefv2_log(level, ...) \
        _libidmefv2_log(level, __FILE__, __LIBIDMEFV2_FUNC__, __LINE__, __VA_ARGS__)

#define libidmefv2_log_debug(level, ...) \
        _libidmefv2_log(LIBIDMEFV2_LOG_DEBUG + level, __FILE__, __LIBIDMEFV2_FUNC__, __LINE__, __VA_ARGS__)
#else

void libidmefv2_log(libidmefv2_log_t level, const char *fmt, ...)
                 LIBIDMEFV2_FMT_PRINTF(2, 3);

void libidmefv2_log_debug(libidmefv2_log_t level, const char *fmt, ...)
                       LIBIDMEFV2_FMT_PRINTF(2, 3);

#endif


#define libidmefv2_log_v(level, fmt, ap) \
        _libidmefv2_log_v(level, __FILE__, __LIBIDMEFV2_FUNC__, __LINE__, fmt, ap)

#define libidmefv2_log_debug_v(level, fmt, ap) \
        _libidmefv2_log_v(LIBIDMEFV2_LOG_DEBUG + level, __FILE__, __LIBIDMEFV2_FUNC__, __LINE__, fmt, ap)


void libidmefv2_log_set_level(libidmefv2_log_t level);

void libidmefv2_log_set_debug_level(int level);

libidmefv2_log_flags_t libidmefv2_log_get_flags(void);

void libidmefv2_log_set_flags(libidmefv2_log_flags_t flags);

char *libidmefv2_log_get_prefix(void);

void libidmefv2_log_set_prefix(char *prefix);

void libidmefv2_log_set_callback(void log_cb(libidmefv2_log_t level, const char *str));

int libidmefv2_log_set_logfile(const char *filename);

void _libidmefv2_log_set_abort_level(libidmefv2_log_t level);

int _libidmefv2_log_set_abort_level_from_string(const char *level);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_LIBIDMEFV2_LOG_H */

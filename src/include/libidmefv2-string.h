/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIDMEFV2_LIBIDMEFV2_STRING_H
#define _LIBIDMEFV2_LIBIDMEFV2_STRING_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdarg.h>

#include "libidmefv2-list.h"
#include "libidmefv2-macros.h"
#include "libidmefv2-inttypes.h"
#include "libidmefv2-linked-object.h"

#ifdef __cplusplus
 extern "C" {
#endif


typedef struct libidmefv2_string libidmefv2_string_t;


int libidmefv2_string_new(libidmefv2_string_t **string);

int libidmefv2_string_new_nodup(libidmefv2_string_t **string, char *str);

int libidmefv2_string_new_ref(libidmefv2_string_t **string, const char *str);

int libidmefv2_string_new_dup(libidmefv2_string_t **string, const char *str);

int libidmefv2_string_new_dup_fast(libidmefv2_string_t **string, const char *str, size_t len);

void libidmefv2_string_destroy(libidmefv2_string_t *string);

void libidmefv2_string_destroy_internal(libidmefv2_string_t *string);

int libidmefv2_string_new_nodup_fast(libidmefv2_string_t **string, char *str, size_t len);

int libidmefv2_string_new_ref_fast(libidmefv2_string_t **string, const char *str, size_t len);

int libidmefv2_string_set_dup_fast(libidmefv2_string_t *string, const char *buf, size_t len);

int libidmefv2_string_set_dup(libidmefv2_string_t *string, const char *buf);

int libidmefv2_string_set_nodup_fast(libidmefv2_string_t *string, char *buf, size_t len);

int libidmefv2_string_set_nodup(libidmefv2_string_t *string, char *buf);

int libidmefv2_string_set_ref_fast(libidmefv2_string_t *string, const char *buf, size_t len);

int libidmefv2_string_set_ref(libidmefv2_string_t *string, const char *buf);

int libidmefv2_string_copy_ref(const libidmefv2_string_t *src, libidmefv2_string_t *dst);

int libidmefv2_string_copy_dup(const libidmefv2_string_t *src, libidmefv2_string_t *dst);

libidmefv2_string_t *libidmefv2_string_ref(libidmefv2_string_t *string);

int libidmefv2_string_clone(const libidmefv2_string_t *src, libidmefv2_string_t **dst);

size_t libidmefv2_string_get_len(const libidmefv2_string_t *string);

const char *libidmefv2_string_get_string_or_default(const libidmefv2_string_t *string, const char *def);

const char *libidmefv2_string_get_string(const libidmefv2_string_t *string);

int libidmefv2_string_get_string_released(libidmefv2_string_t *string, char **outptr);

libidmefv2_bool_t libidmefv2_string_is_empty(const libidmefv2_string_t *string);

int libidmefv2_string_truncate(libidmefv2_string_t *string, size_t len);

void libidmefv2_string_clear(libidmefv2_string_t *string);

/*
 * string operation
 */
int libidmefv2_string_cat(libidmefv2_string_t *dst, const char *str);
int libidmefv2_string_ncat(libidmefv2_string_t *dst, const char *str, size_t len);

int libidmefv2_string_sprintf(libidmefv2_string_t *string, const char *fmt, ...)
                           LIBIDMEFV2_FMT_PRINTF(2, 3);

int libidmefv2_string_vprintf(libidmefv2_string_t *string, const char *fmt, va_list ap)
                           LIBIDMEFV2_FMT_PRINTF(2, 0);

int libidmefv2_string_compare(const libidmefv2_string_t *str1, const libidmefv2_string_t *str2);

#define libidmefv2_string_set_constant(string, str)                         \
        libidmefv2_string_set_ref_fast((string), (str), strlen((str)))

#define libidmefv2_string_new_constant(string, str)                         \
        libidmefv2_string_new_ref_fast((string), (str), strlen((str)))

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_LIBIDMEFV2_STRING_H */

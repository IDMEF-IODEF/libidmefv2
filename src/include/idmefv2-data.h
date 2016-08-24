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

#ifndef _LIBIDMEFV2_IDMEFV2_DATA_H
#define _LIBIDMEFV2_IDMEFV2_DATA_H

#include "libidmefv2-list.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IDMEFV2_DATA_TYPE_UNKNOWN      = 0,
        IDMEFV2_DATA_TYPE_CHAR         = 1,
        IDMEFV2_DATA_TYPE_BYTE         = 2,
        IDMEFV2_DATA_TYPE_UINT32       = 3,
        IDMEFV2_DATA_TYPE_UINT64       = 4,
        IDMEFV2_DATA_TYPE_INT          = 4,
        IDMEFV2_DATA_TYPE_FLOAT        = 5,
        IDMEFV2_DATA_TYPE_CHAR_STRING  = 6,
        IDMEFV2_DATA_TYPE_BYTE_STRING  = 7,
        IDMEFV2_DATA_TYPE_TIME         = 8
} idmefv2_data_type_t;


typedef struct {
        int refcount;

        int flags;
        idmefv2_data_type_t type;
        size_t len;

        union {
                char char_data;
                uint8_t byte_data;
                int64_t int_data;
                float float_data;
                void *rw_data;
                const void *ro_data;
        } data;

        libidmefv2_list_t list;
} idmefv2_data_t;



int idmefv2_data_new(idmefv2_data_t **data);

idmefv2_data_t *idmefv2_data_ref(idmefv2_data_t *data);


int idmefv2_data_new_char(idmefv2_data_t **data, char c);
int idmefv2_data_new_byte(idmefv2_data_t **data, uint8_t i);
int idmefv2_data_new_uint32(idmefv2_data_t **data, uint32_t i) LIBIDMEFV2_DEPRECATED;
int idmefv2_data_new_uint64(idmefv2_data_t **data, uint64_t i) LIBIDMEFV2_DEPRECATED;
int idmefv2_data_new_int(idmefv2_data_t **data, int64_t i);
int idmefv2_data_new_float(idmefv2_data_t **data, float f);
int idmefv2_data_new_time(idmefv2_data_t **data, idmefv2_time_t *time);

void idmefv2_data_set_char(idmefv2_data_t *data, char c);
void idmefv2_data_set_byte(idmefv2_data_t *data, uint8_t i);
void idmefv2_data_set_uint32(idmefv2_data_t *data, uint32_t i) LIBIDMEFV2_DEPRECATED;
void idmefv2_data_set_uint64(idmefv2_data_t *data, uint64_t i) LIBIDMEFV2_DEPRECATED;
void idmefv2_data_set_int(idmefv2_data_t *data, int64_t i);
void idmefv2_data_set_float(idmefv2_data_t *data, float f);
void idmefv2_data_set_time(idmefv2_data_t *data, idmefv2_time_t *time);

int idmefv2_data_set_ptr_dup_fast(idmefv2_data_t *data, idmefv2_data_type_t type, const void *ptr, size_t len);
int idmefv2_data_set_ptr_ref_fast(idmefv2_data_t *data, idmefv2_data_type_t type, const void *ptr, size_t len);
int idmefv2_data_set_ptr_nodup_fast(idmefv2_data_t *data, idmefv2_data_type_t type, void *ptr, size_t len);

int idmefv2_data_new_ptr_dup_fast(idmefv2_data_t **data, idmefv2_data_type_t type, const void *ptr, size_t len);
int idmefv2_data_new_ptr_ref_fast(idmefv2_data_t **data, idmefv2_data_type_t type, const void *ptr, size_t len);
int idmefv2_data_new_ptr_nodup_fast(idmefv2_data_t **data, idmefv2_data_type_t type, void *ptr, size_t len);


/*
 * String functions
 */
int idmefv2_data_set_char_string_dup_fast(idmefv2_data_t *data, const char *str, size_t len);
int idmefv2_data_new_char_string_dup_fast(idmefv2_data_t **data, const char *str, size_t len);
int idmefv2_data_new_char_string_ref_fast(idmefv2_data_t **data, const char *ptr, size_t len);
int idmefv2_data_new_char_string_nodup_fast(idmefv2_data_t **data, char *ptr, size_t len);
int idmefv2_data_set_char_string_ref_fast(idmefv2_data_t *data, const char *ptr, size_t len);
int idmefv2_data_set_char_string_nodup_fast(idmefv2_data_t *data, char *ptr, size_t len);
int idmefv2_data_new_char_string_ref(idmefv2_data_t **data, const char *ptr);
int idmefv2_data_new_char_string_dup(idmefv2_data_t **data, const char *ptr);
int idmefv2_data_new_char_string_nodup(idmefv2_data_t **data, char *ptr);
int idmefv2_data_set_char_string_ref(idmefv2_data_t *data, const char *ptr);
int idmefv2_data_set_char_string_dup(idmefv2_data_t *data, const char *ptr);
int idmefv2_data_set_char_string_nodup(idmefv2_data_t *data, char *ptr);

#define idmefv2_data_set_char_string_constant(string, str)                \
        idmefv2_data_set_char_string_ref_fast((string), (str), sizeof((str)) - 1)


/*
 * Byte functions
 */
int idmefv2_data_new_byte_string_ref(idmefv2_data_t **data, const unsigned char *ptr, size_t len);
int idmefv2_data_new_byte_string_dup(idmefv2_data_t **data, const unsigned char *ptr, size_t len);
int idmefv2_data_new_byte_string_nodup(idmefv2_data_t **data, unsigned char *ptr, size_t len);
int idmefv2_data_set_byte_string_ref(idmefv2_data_t *data, const unsigned char *ptr, size_t len);
int idmefv2_data_set_byte_string_dup(idmefv2_data_t *data, const unsigned char *ptr, size_t len);
int idmefv2_data_set_byte_string_nodup(idmefv2_data_t *data, unsigned char *ptr, size_t len);



/*
 *
 */

void idmefv2_data_destroy(idmefv2_data_t *data);

int idmefv2_data_copy_ref(const idmefv2_data_t *src, idmefv2_data_t *dst);

int idmefv2_data_copy_dup(const idmefv2_data_t *src, idmefv2_data_t *dst);

int idmefv2_data_clone(const idmefv2_data_t *src, idmefv2_data_t **dst);

idmefv2_data_type_t idmefv2_data_get_type(const idmefv2_data_t *data);

size_t idmefv2_data_get_len(const idmefv2_data_t *data);

const void *idmefv2_data_get_data(const idmefv2_data_t *data);

char idmefv2_data_get_char(const idmefv2_data_t *data);

uint8_t idmefv2_data_get_byte(const idmefv2_data_t *data);

uint32_t idmefv2_data_get_uint32(const idmefv2_data_t *data) LIBIDMEFV2_DEPRECATED;

uint64_t idmefv2_data_get_uint64(const idmefv2_data_t *data) LIBIDMEFV2_DEPRECATED;

int64_t idmefv2_data_get_int(const idmefv2_data_t *data);

float idmefv2_data_get_float(const idmefv2_data_t *data);

const char *idmefv2_data_get_char_string(const idmefv2_data_t *data);

const unsigned char *idmefv2_data_get_byte_string(const idmefv2_data_t *data);

libidmefv2_bool_t idmefv2_data_is_empty(const idmefv2_data_t *data);

int idmefv2_data_to_string(const idmefv2_data_t *data, libidmefv2_string_t *out);

void idmefv2_data_destroy_internal(idmefv2_data_t *data);

int idmefv2_data_compare(const idmefv2_data_t *data1, const idmefv2_data_t *data2);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_IDMEFV2_DATA_H */

/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
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

#ifndef _IDMEFV2_VALUE_TYPE_H
#define _IDMEFV2_VALUE_TYPE_H

#include "idmefv2-time.h"
#include "idmefv2-data.h"
#include "libidmefv2-string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IDMEFV2_VALUE_TYPE_ERROR   =  -1,
        IDMEFV2_VALUE_TYPE_UNKNOWN =   0,
        IDMEFV2_VALUE_TYPE_INT8    =   1,
        IDMEFV2_VALUE_TYPE_UINT8   =   2,
        IDMEFV2_VALUE_TYPE_INT16   =   3,
        IDMEFV2_VALUE_TYPE_UINT16  =   4,
        IDMEFV2_VALUE_TYPE_INT32   =   5,
        IDMEFV2_VALUE_TYPE_UINT32  =   6,
        IDMEFV2_VALUE_TYPE_INT64   =   7,
        IDMEFV2_VALUE_TYPE_UINT64  =   8,
        IDMEFV2_VALUE_TYPE_FLOAT   =   9,
        IDMEFV2_VALUE_TYPE_DOUBLE  =  10,
        IDMEFV2_VALUE_TYPE_STRING  =  11,
        IDMEFV2_VALUE_TYPE_TIME    =  12,
        IDMEFV2_VALUE_TYPE_DATA    =  13,
        IDMEFV2_VALUE_TYPE_ENUM    =  14,
        IDMEFV2_VALUE_TYPE_LIST    =  15,
        IDMEFV2_VALUE_TYPE_CLASS   =  16
} idmefv2_value_type_id_t;


typedef struct {
        void *object;
        int class_id;
} idmefv2_value_type_class_t;

typedef struct {
        int value;
        int class_id;
} idmefv2_value_type_enum_t;


typedef union {
        int8_t int8_val;
        uint8_t uint8_val;
        int16_t int16_val;
        uint16_t uint16_val;
        int32_t int32_val;
        uint32_t uint32_val;
        int64_t int64_val;
        uint64_t uint64_val;
        float float_val;
        double double_val;
        libidmefv2_string_t *string_val;
        idmefv2_time_t *time_val;
        idmefv2_data_t *data_val;
        libidmefv2_list_t list_val;
        idmefv2_value_type_enum_t enum_val;
        idmefv2_value_type_class_t class_val;
} idmefv2_value_type_data_t;


typedef struct {
        idmefv2_value_type_id_t id;
        idmefv2_value_type_data_t data;
} idmefv2_value_type_t;


#include "idmefv2-criteria.h"

int idmefv2_value_type_ref(const idmefv2_value_type_t *src);

int idmefv2_value_type_copy(const idmefv2_value_type_t *src, void *dst);

int idmefv2_value_type_read(idmefv2_value_type_t *dst, const char *buf);

int idmefv2_value_type_write(const idmefv2_value_type_t *src, libidmefv2_string_t *out);

void idmefv2_value_type_destroy(idmefv2_value_type_t *type);

int idmefv2_value_type_clone(const idmefv2_value_type_t *src, idmefv2_value_type_t *dst);

int idmefv2_value_type_compare(const idmefv2_value_type_t *type1, const idmefv2_value_type_t *type2,
                             idmefv2_criterion_operator_t op);

int idmefv2_value_type_check_operator(idmefv2_value_type_id_t type, idmefv2_criterion_operator_t op);

int idmefv2_value_type_get_applicable_operators(idmefv2_value_type_id_t type, idmefv2_criterion_operator_t *result);

const char *idmefv2_value_type_to_string(idmefv2_value_type_id_t type);

#ifdef __cplusplus
 }
#endif

#endif

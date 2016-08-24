/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@libidmefv2-ids.com>
* Author: Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
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

#ifndef _LIBIDMEFV2_IDMEFV2_VALUE_H
#define _LIBIDMEFV2_IDMEFV2_VALUE_H


typedef struct idmefv2_value idmefv2_value_t;

#include "libidmefv2-io.h"
#include "idmefv2-value-type.h"
#include "libidmefv2-string.h"
#include "idmefv2-class.h"
#include "idmefv2-path.h"

#ifdef __cplusplus
 extern "C" {
#endif

int idmefv2_value_new_int8(idmefv2_value_t **value, int8_t val);
int idmefv2_value_new_uint8(idmefv2_value_t **value, uint8_t val);
int idmefv2_value_new_int16(idmefv2_value_t **value, int16_t val);
int idmefv2_value_new_uint16(idmefv2_value_t **value, uint16_t val);
int idmefv2_value_new_int32(idmefv2_value_t **value, int32_t val);
int idmefv2_value_new_uint32(idmefv2_value_t **value, uint32_t val);
int idmefv2_value_new_int64(idmefv2_value_t **value, int64_t val);
int idmefv2_value_new_uint64(idmefv2_value_t **value, uint64_t val);
int idmefv2_value_new_float(idmefv2_value_t **value, float val);
int idmefv2_value_new_double(idmefv2_value_t **value, double val);
int idmefv2_value_new_string(idmefv2_value_t **value, libidmefv2_string_t *string);
int idmefv2_value_new_time(idmefv2_value_t **value, idmefv2_time_t *time);
int idmefv2_value_new_data(idmefv2_value_t **value, idmefv2_data_t *data);
int idmefv2_value_new_class(idmefv2_value_t **value, idmefv2_class_id_t classid, void *ptr);
int idmefv2_value_new_list(idmefv2_value_t **value);
int idmefv2_value_new_enum(idmefv2_value_t **value, idmefv2_class_id_t classid, const char *buf);
int idmefv2_value_new_enum_from_string(idmefv2_value_t **value, idmefv2_class_id_t classid, const char *buf);
int idmefv2_value_new_enum_from_numeric(idmefv2_value_t **value, idmefv2_class_id_t classid, int val);

int idmefv2_value_set_int8(idmefv2_value_t *value, int8_t val);
int idmefv2_value_set_uint8(idmefv2_value_t *value, uint8_t val);
int idmefv2_value_set_int16(idmefv2_value_t *value, int16_t val);
int idmefv2_value_set_uint16(idmefv2_value_t *value, uint16_t val);
int idmefv2_value_set_int32(idmefv2_value_t *value, int32_t val);
int idmefv2_value_set_uint32(idmefv2_value_t *value, uint32_t val);
int idmefv2_value_set_int64(idmefv2_value_t *value, int64_t val);
int idmefv2_value_set_uint64(idmefv2_value_t *value, uint64_t val);
int idmefv2_value_set_float(idmefv2_value_t *value, float val);
int idmefv2_value_set_double(idmefv2_value_t *value, double val);
int idmefv2_value_set_string(idmefv2_value_t *value, libidmefv2_string_t *string);
int idmefv2_value_set_time(idmefv2_value_t *value, idmefv2_time_t *time);
int idmefv2_value_set_data(idmefv2_value_t *value, idmefv2_data_t *data);
int idmefv2_value_set_enum(idmefv2_value_t *value, idmefv2_class_id_t classid, const char *buf);
int idmefv2_value_set_enum_from_string(idmefv2_value_t *value, idmefv2_class_id_t classid, const char *buf);
int idmefv2_value_set_enum_from_numeric(idmefv2_value_t *value, idmefv2_class_id_t classid, int no);
int idmefv2_value_set_class(idmefv2_value_t *value, idmefv2_class_id_t classid, void *ptr);

int idmefv2_value_new(idmefv2_value_t **value, idmefv2_value_type_id_t type, void *ptr);
int idmefv2_value_new_from_path(idmefv2_value_t **value, idmefv2_path_t *path, const char *buf);
int idmefv2_value_new_from_string(idmefv2_value_t **value, idmefv2_value_type_id_t type, const char *buf);

int8_t idmefv2_value_get_int8(const idmefv2_value_t *val);
uint8_t idmefv2_value_get_uint8(const idmefv2_value_t *val);
int16_t idmefv2_value_get_int16(const idmefv2_value_t *val);
uint16_t idmefv2_value_get_uint16(const idmefv2_value_t *val);
int32_t idmefv2_value_get_int32(const idmefv2_value_t *val);
uint32_t idmefv2_value_get_uint32(const idmefv2_value_t *val);
int64_t idmefv2_value_get_int64(const idmefv2_value_t *val);
uint64_t idmefv2_value_get_uint64(const idmefv2_value_t *val);
int idmefv2_value_get_enum(const idmefv2_value_t *val);
float idmefv2_value_get_float(const idmefv2_value_t *val);
double idmefv2_value_get_double(const idmefv2_value_t *val);

idmefv2_time_t *idmefv2_value_get_time(const idmefv2_value_t *val);
idmefv2_data_t *idmefv2_value_get_data(const idmefv2_value_t *val);
libidmefv2_string_t *idmefv2_value_get_string(const idmefv2_value_t *val);

int idmefv2_value_list_add(idmefv2_value_t *list, idmefv2_value_t *item);
libidmefv2_bool_t idmefv2_value_is_list(const idmefv2_value_t *list);
libidmefv2_bool_t idmefv2_value_list_is_empty(const idmefv2_value_t *list);

int idmefv2_value_have_own_data(idmefv2_value_t *value);
int idmefv2_value_dont_have_own_data(idmefv2_value_t *value);

idmefv2_value_type_id_t idmefv2_value_get_type(const idmefv2_value_t *value);
idmefv2_class_id_t idmefv2_value_get_class(const idmefv2_value_t *value);

void *idmefv2_value_get_object(const idmefv2_value_t *value);

int idmefv2_value_iterate(const idmefv2_value_t *value, int (*callback)(idmefv2_value_t *ptr, void *extra), void *extra);

int idmefv2_value_iterate_reversed(const idmefv2_value_t *value, int (*callback)(idmefv2_value_t *ptr, void *extra), void *extra);

int idmefv2_value_get_nth2(const idmefv2_value_t *val, int index, idmefv2_value_t **ret);

idmefv2_value_t *idmefv2_value_get_nth(const idmefv2_value_t *val, int n);

int idmefv2_value_get_count(const idmefv2_value_t *val);

int idmefv2_value_clone(const idmefv2_value_t *val, idmefv2_value_t **dst);

idmefv2_value_t *idmefv2_value_ref(idmefv2_value_t *val);

int idmefv2_value_print(const idmefv2_value_t *val, libidmefv2_io_t *fd);

int idmefv2_value_to_string(const idmefv2_value_t *value, libidmefv2_string_t *out);

int idmefv2_value_get(const idmefv2_value_t *val, void *res);

int idmefv2_value_match(idmefv2_value_t *val1, idmefv2_value_t *val2, idmefv2_criterion_operator_t op);

int idmefv2_value_check_operator(const idmefv2_value_t *value, idmefv2_criterion_operator_t op);

int idmefv2_value_get_applicable_operators(const idmefv2_value_t *value, idmefv2_criterion_operator_t *result);

void idmefv2_value_destroy(idmefv2_value_t *val);

#ifndef SWIG

int _idmefv2_value_copy_internal(const idmefv2_value_t *val,
                               idmefv2_value_type_id_t res_type, idmefv2_class_id_t res_id, void *res);

int _idmefv2_value_cast(idmefv2_value_t *val, idmefv2_value_type_id_t target_type, idmefv2_class_id_t enum_class);
#endif

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_IDMEFV2_VALUE_H */

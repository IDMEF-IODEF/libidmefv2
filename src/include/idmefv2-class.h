/*****
*
* Copyright (C) 2002, 2003, 2004 Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
* All Rights Reserved
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


#ifndef _LIBIDMEFV2_IDMEFV2_CLASS_H
#define _LIBIDMEFV2_IDMEFV2_CLASS_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef int idmefv2_class_id_t;
typedef int idmefv2_class_child_id_t;

#include "idmefv2-value.h"

/*
 *
 */
libidmefv2_bool_t idmefv2_class_is_child_list(idmefv2_class_id_t classid, idmefv2_class_child_id_t child);

libidmefv2_bool_t idmefv2_class_is_child_keyed_list(idmefv2_class_id_t classid, idmefv2_class_child_id_t child);

libidmefv2_bool_t idmefv2_class_is_child_union_member(idmefv2_class_id_t classid, idmefv2_class_child_id_t child);

int idmefv2_class_get_child_union_id(idmefv2_class_id_t classid, idmefv2_class_child_id_t child);

idmefv2_class_id_t idmefv2_class_get_child_class(idmefv2_class_id_t classid, idmefv2_class_child_id_t child);

size_t idmefv2_class_get_child_count(idmefv2_class_id_t classid);

idmefv2_value_type_id_t idmefv2_class_get_child_value_type(idmefv2_class_id_t classid, idmefv2_class_child_id_t child);


/*
 *
 */
int idmefv2_class_enum_to_numeric(idmefv2_class_id_t classid, const char *val);

const char *idmefv2_class_enum_to_string(idmefv2_class_id_t classid, int val);


/*
 *
 */
int idmefv2_class_get_child(void *ptr, idmefv2_class_id_t classid, idmefv2_class_child_id_t child, void **childptr);

int idmefv2_class_new_child(void *ptr, idmefv2_class_id_t classid, idmefv2_class_child_id_t child, int n, void **childptr);

int idmefv2_class_destroy_child(void *ptr, idmefv2_class_id_t classid, idmefv2_class_child_id_t child, int n);


/*
 *
 */
idmefv2_class_id_t idmefv2_class_find(const char *name);

idmefv2_class_child_id_t idmefv2_class_find_child(idmefv2_class_id_t classid, const char *name);

/*
 *
 */
const char *idmefv2_class_get_name(idmefv2_class_id_t classid);

const char *idmefv2_class_get_child_name(idmefv2_class_id_t classid, idmefv2_class_child_id_t child);


/*
 *
 */
int idmefv2_class_copy(idmefv2_class_id_t classid, const void *src, void *dst);

int idmefv2_class_clone(idmefv2_class_id_t classid, const void *src, void **dst);

int idmefv2_class_compare(idmefv2_class_id_t classid, const void *c1, const void *c2);

int idmefv2_class_ref(idmefv2_class_id_t classid, void *obj);

int idmefv2_class_print(idmefv2_class_id_t classid, void *obj, libidmefv2_io_t *fd);

int idmefv2_class_print_json(idmefv2_class_id_t classid, void *obj, libidmefv2_io_t *fd);

int idmefv2_class_print_binary(idmefv2_class_id_t classid, void *obj, libidmefv2_io_t *fd);

int idmefv2_class_destroy(idmefv2_class_id_t classid, void *obj);

libidmefv2_bool_t idmefv2_class_is_listed(idmefv2_class_id_t classid);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_IDMEFV2_CLASS_H */

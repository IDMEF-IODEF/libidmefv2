/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
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

#ifndef _LIBIDMEFV2_IDMEFV2_OBJECT_H
#define _LIBIDMEFV2_IDMEFV2_OBJECT_H

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libidmefv2-io.h"
#include "libidmefv2-list.h"

typedef struct idmefv2_object idmefv2_object_t;

idmefv2_class_id_t idmefv2_object_get_class(idmefv2_object_t *obj);

idmefv2_object_t *idmefv2_object_ref(idmefv2_object_t *obj);

void idmefv2_object_destroy(idmefv2_object_t *obj);

int idmefv2_object_compare(idmefv2_object_t *obj1, idmefv2_object_t *obj2);

int idmefv2_object_clone(idmefv2_object_t *obj, idmefv2_object_t **dst);

int idmefv2_object_copy(idmefv2_object_t *src, idmefv2_object_t *dst);

int idmefv2_object_print(idmefv2_object_t *obj, libidmefv2_io_t *fd);

int idmefv2_object_print_json(idmefv2_object_t *obj, libidmefv2_io_t *fd);

int idmefv2_object_print_binary(idmefv2_object_t *obj, libidmefv2_io_t *fd);

void idmefv2_object_add(libidmefv2_list_t *head, idmefv2_object_t *obj);

void idmefv2_object_add_tail(libidmefv2_list_t *head, idmefv2_object_t *obj);

void idmefv2_object_del(idmefv2_object_t *object);

void idmefv2_object_del_init(idmefv2_object_t *object);

void *idmefv2_object_get_list_entry(libidmefv2_list_t *listm);

int idmefv2_object_new_from_json(idmefv2_object_t **object, const char * json_message);

#ifdef __cplusplus
  }
#endif

#endif /* _LIBIDMEFV2_IDMEFV2_OBJECT_H */

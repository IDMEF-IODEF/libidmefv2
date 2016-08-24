/*****
*
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libidmefv2-ids.com>
* Author: Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
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

#ifndef _LIBIDMEFV2_IDMEFV2_PATH_H
#define _LIBIDMEFV2_IDMEFV2_PATH_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

typedef struct idmefv2_path idmefv2_path_t;

#include <stdarg.h>
#include "libidmefv2-macros.h"
#include "idmefv2-value.h"
#include "idmefv2-tree-wrap.h"

int idmefv2_path_get(const idmefv2_path_t *path, void *object, idmefv2_value_t **ret);

int idmefv2_path_set(const idmefv2_path_t *path, void *object, idmefv2_value_t *value);

int idmefv2_path_new(idmefv2_path_t **path, const char *format, ...) LIBIDMEFV2_FMT_PRINTF(2, 3);

int idmefv2_path_new_v(idmefv2_path_t **path, const char *format, va_list args) LIBIDMEFV2_FMT_PRINTF(2, 0);

int idmefv2_path_new_from_root_fast(idmefv2_path_t **path, idmefv2_class_id_t rootclass, const char *buffer);

int idmefv2_path_new_fast(idmefv2_path_t **path, const char *buffer);

idmefv2_class_id_t idmefv2_path_get_class(const idmefv2_path_t *path, int depth);

idmefv2_value_type_id_t idmefv2_path_get_value_type(const idmefv2_path_t *path, int depth);

int idmefv2_path_set_index(idmefv2_path_t *path, unsigned int depth, int index);

int idmefv2_path_undefine_index(idmefv2_path_t *path, unsigned int depth);

int idmefv2_path_get_index(const idmefv2_path_t *path, unsigned int depth);

int idmefv2_path_get_key(const idmefv2_path_t *path, unsigned int depth, const char **key);

int idmefv2_path_make_child(idmefv2_path_t *path, const char *child_name, int index);

int idmefv2_path_make_parent(idmefv2_path_t *path);

void idmefv2_path_destroy(idmefv2_path_t *path);

int idmefv2_path_ncompare(const idmefv2_path_t *p1, const idmefv2_path_t *p2, unsigned int depth);

int idmefv2_path_compare(const idmefv2_path_t *p1, const idmefv2_path_t *p2);

int idmefv2_path_clone(const idmefv2_path_t *src, idmefv2_path_t **dst);

idmefv2_path_t *idmefv2_path_ref(idmefv2_path_t *path);

const char *idmefv2_path_get_name(const idmefv2_path_t *path, int depth);

libidmefv2_bool_t idmefv2_path_is_ambiguous(const idmefv2_path_t *path);

int idmefv2_path_has_lists(const idmefv2_path_t *path);

libidmefv2_bool_t idmefv2_path_is_list(const idmefv2_path_t *path, int depth);

unsigned int idmefv2_path_get_depth(const idmefv2_path_t *path);

int idmefv2_path_check_operator(const idmefv2_path_t *path, idmefv2_criterion_operator_t op);

int idmefv2_path_get_applicable_operators(const idmefv2_path_t *path, idmefv2_criterion_operator_t *result);

#ifndef SWIG
void _idmefv2_path_cache_lock(void);

void _idmefv2_path_cache_reinit(void);

void _idmefv2_path_cache_unlock(void);

void _idmefv2_path_cache_destroy(void);
#endif

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_IDMEFV2_PATH_H */

/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIDMEFV2_IDMEFV2_CRITERION_VALUE_H
#define _LIBIDMEFV2_IDMEFV2_CRITERION_VALUE_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct idmefv2_criterion_value idmefv2_criterion_value_t;

typedef enum {
        IDMEFV2_CRITERION_VALUE_TYPE_ERROR            = -1,
        IDMEFV2_CRITERION_VALUE_TYPE_VALUE            =  0,
        IDMEFV2_CRITERION_VALUE_TYPE_REGEX            =  1,
        IDMEFV2_CRITERION_VALUE_TYPE_BROKEN_DOWN_TIME =  2
} idmefv2_criterion_value_type_t;

         
#include "idmefv2-criteria.h"
#include "idmefv2-value.h"
 

int idmefv2_criterion_value_new(idmefv2_criterion_value_t **cv);

int idmefv2_criterion_value_new_regex(idmefv2_criterion_value_t **cv, const char *regex, idmefv2_criterion_operator_t op);

int idmefv2_criterion_value_new_value(idmefv2_criterion_value_t **cv, idmefv2_value_t *value,
                                    idmefv2_criterion_operator_t op);

int idmefv2_criterion_value_new_from_string(idmefv2_criterion_value_t **cv, idmefv2_path_t *path,
                                          const char *value, idmefv2_criterion_operator_t op);

int idmefv2_criterion_value_new_broken_down_time(idmefv2_criterion_value_t **cv, const char *time, idmefv2_criterion_operator_t op);
         
int idmefv2_criterion_value_clone(const idmefv2_criterion_value_t *src, idmefv2_criterion_value_t **dst);

void idmefv2_criterion_value_destroy(idmefv2_criterion_value_t *value);

int idmefv2_criterion_value_print(idmefv2_criterion_value_t *value, libidmefv2_io_t *fd);

int idmefv2_criterion_value_to_string(idmefv2_criterion_value_t *value, libidmefv2_string_t *out);

int idmefv2_criterion_value_match(idmefv2_criterion_value_t *cv, idmefv2_value_t *value, idmefv2_criterion_operator_t op);

const idmefv2_value_t *idmefv2_criterion_value_get_value(idmefv2_criterion_value_t *cv);

const char *idmefv2_criterion_value_get_regex(idmefv2_criterion_value_t *cv);

const struct tm *idmefv2_criterion_value_get_broken_down_time(idmefv2_criterion_value_t *cv);
         
idmefv2_criterion_value_type_t idmefv2_criterion_value_get_type(idmefv2_criterion_value_t *cv);

#ifdef __cplusplus
 }
#endif

         
#endif /* _LIBIDMEFV2_IDMEFV2_CRITERION_VALUE_H */

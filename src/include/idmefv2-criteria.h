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

#ifndef _LIBIDMEFV2_IDMEFV2_CRITERIA_H
#define _LIBIDMEFV2_IDMEFV2_CRITERIA_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IDMEFV2_CRITERION_OPERATOR_NOT               = 0x8000,
        IDMEFV2_CRITERION_OPERATOR_NOCASE            = 0x4000,

        IDMEFV2_CRITERION_OPERATOR_EQUAL             = 0x0001,
        IDMEFV2_CRITERION_OPERATOR_EQUAL_NOCASE      = IDMEFV2_CRITERION_OPERATOR_EQUAL|IDMEFV2_CRITERION_OPERATOR_NOCASE,
        IDMEFV2_CRITERION_OPERATOR_NOT_EQUAL         = IDMEFV2_CRITERION_OPERATOR_NOT|IDMEFV2_CRITERION_OPERATOR_EQUAL,
        IDMEFV2_CRITERION_OPERATOR_NOT_EQUAL_NOCASE  = IDMEFV2_CRITERION_OPERATOR_NOT_EQUAL|IDMEFV2_CRITERION_OPERATOR_EQUAL_NOCASE,

        IDMEFV2_CRITERION_OPERATOR_LESSER            = 0x0002,
        IDMEFV2_CRITERION_OPERATOR_LESSER_OR_EQUAL   = IDMEFV2_CRITERION_OPERATOR_LESSER|IDMEFV2_CRITERION_OPERATOR_EQUAL,

        IDMEFV2_CRITERION_OPERATOR_GREATER           = 0x0004,
        IDMEFV2_CRITERION_OPERATOR_GREATER_OR_EQUAL  = IDMEFV2_CRITERION_OPERATOR_GREATER|IDMEFV2_CRITERION_OPERATOR_EQUAL,

        IDMEFV2_CRITERION_OPERATOR_SUBSTR            = 0x0008,
        IDMEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE     = IDMEFV2_CRITERION_OPERATOR_SUBSTR|IDMEFV2_CRITERION_OPERATOR_NOCASE,
        IDMEFV2_CRITERION_OPERATOR_NOT_SUBSTR        = IDMEFV2_CRITERION_OPERATOR_NOT|IDMEFV2_CRITERION_OPERATOR_SUBSTR,
        IDMEFV2_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE = IDMEFV2_CRITERION_OPERATOR_NOT|IDMEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE,

        IDMEFV2_CRITERION_OPERATOR_REGEX             = 0x0010,
        IDMEFV2_CRITERION_OPERATOR_REGEX_NOCASE      = IDMEFV2_CRITERION_OPERATOR_REGEX|IDMEFV2_CRITERION_OPERATOR_NOCASE,
        IDMEFV2_CRITERION_OPERATOR_NOT_REGEX         = IDMEFV2_CRITERION_OPERATOR_NOT|IDMEFV2_CRITERION_OPERATOR_REGEX,
        IDMEFV2_CRITERION_OPERATOR_NOT_REGEX_NOCASE  = IDMEFV2_CRITERION_OPERATOR_NOT|IDMEFV2_CRITERION_OPERATOR_REGEX_NOCASE,

        IDMEFV2_CRITERION_OPERATOR_NULL              = 0x0020,
        IDMEFV2_CRITERION_OPERATOR_NOT_NULL          = IDMEFV2_CRITERION_OPERATOR_NULL|IDMEFV2_CRITERION_OPERATOR_NOT
} idmefv2_criterion_operator_t;


typedef struct idmefv2_criteria idmefv2_criteria_t;
typedef struct idmefv2_criterion idmefv2_criterion_t;

#include "idmefv2-path.h"
#include "idmefv2-criterion-value.h"

const char *idmefv2_criterion_operator_to_string(idmefv2_criterion_operator_t op);

int idmefv2_criterion_new(idmefv2_criterion_t **criterion, idmefv2_path_t *path,
                        idmefv2_criterion_value_t *value, idmefv2_criterion_operator_t op);

void idmefv2_criterion_destroy(idmefv2_criterion_t *criterion);
idmefv2_criteria_t *idmefv2_criteria_ref(idmefv2_criteria_t *criteria);
int idmefv2_criterion_clone(const idmefv2_criterion_t *criterion, idmefv2_criterion_t **dst);
int idmefv2_criterion_print(const idmefv2_criterion_t *criterion, libidmefv2_io_t *fd);
int idmefv2_criterion_to_string(const idmefv2_criterion_t *criterion, libidmefv2_string_t *out);
idmefv2_path_t *idmefv2_criterion_get_path(const idmefv2_criterion_t *criterion);
idmefv2_criterion_value_t *idmefv2_criterion_get_value(const idmefv2_criterion_t *criterion);
idmefv2_criterion_operator_t idmefv2_criterion_get_operator(const idmefv2_criterion_t *criterion);
int idmefv2_criterion_match(const idmefv2_criterion_t *criterion, void *object);

int idmefv2_criteria_new(idmefv2_criteria_t **criteria);
void idmefv2_criteria_destroy(idmefv2_criteria_t *criteria);
int idmefv2_criteria_clone(idmefv2_criteria_t *src, idmefv2_criteria_t **dst);
int idmefv2_criteria_print(const idmefv2_criteria_t *criteria, libidmefv2_io_t *fd);
int idmefv2_criteria_to_string(const idmefv2_criteria_t *criteria, libidmefv2_string_t *out);
libidmefv2_bool_t idmefv2_criteria_is_criterion(const idmefv2_criteria_t *criteria);
idmefv2_criterion_t *idmefv2_criteria_get_criterion(const idmefv2_criteria_t *criteria);
void idmefv2_criteria_set_criterion(idmefv2_criteria_t *criteria, idmefv2_criterion_t *criterion);

void idmefv2_criteria_or_criteria(idmefv2_criteria_t *criteria, idmefv2_criteria_t *criteria2);

int idmefv2_criteria_and_criteria(idmefv2_criteria_t *criteria, idmefv2_criteria_t *criteria2);

int idmefv2_criteria_match(const idmefv2_criteria_t *criteria, void *object);

idmefv2_criteria_t *idmefv2_criteria_get_or(const idmefv2_criteria_t *criteria);

idmefv2_criteria_t *idmefv2_criteria_get_and(const idmefv2_criteria_t *criteria);

int idmefv2_criteria_new_from_string(idmefv2_criteria_t **criteria, const char *str);

void idmefv2_criteria_set_negation(idmefv2_criteria_t *criteria, libidmefv2_bool_t negate);

libidmefv2_bool_t idmefv2_criteria_get_negation(const idmefv2_criteria_t *criteria);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_IDMEFV2_CRITERIA_H */

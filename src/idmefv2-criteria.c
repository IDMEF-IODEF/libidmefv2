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

#include "config.h"
#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>

#define LIBIDMEFV2_ERROR_SOURCE_DEFAULT LIBIDMEFV2_ERROR_SOURCE_IDMEFV2_CRITERIA
#include "idmefv2.h"
#include "idmefv2-criteria.h"


struct idmefv2_criterion {
        idmefv2_path_t *path;
        idmefv2_criterion_value_t *value;
        idmefv2_criterion_operator_t operator;
};


struct idmefv2_criteria {
        int refcount;
        libidmefv2_bool_t negated;
        idmefv2_criterion_t *criterion;
        struct idmefv2_criteria *or;
        struct idmefv2_criteria *and;
};



/**
 * idmefv2_criterion_operator_to_string:
 * @op: #idmefv2_criterion_operator_t type.
 *
 * Transforms @op to string.
 *
 * Returns: A pointer to an operator string or NULL.
 */
const char *idmefv2_criterion_operator_to_string(idmefv2_criterion_operator_t op)
{
        int i;
        const struct {
                idmefv2_criterion_operator_t operator;
                const char *name;
        } tbl[] = {
                { IDMEFV2_CRITERION_OPERATOR_EQUAL,     "="            },
                { IDMEFV2_CRITERION_OPERATOR_EQUAL_NOCASE, "=*"        },

                { IDMEFV2_CRITERION_OPERATOR_NOT_EQUAL, "!="           },
                { IDMEFV2_CRITERION_OPERATOR_NOT_EQUAL_NOCASE, "!=*"   },

                { IDMEFV2_CRITERION_OPERATOR_LESSER, "<"               },
                { IDMEFV2_CRITERION_OPERATOR_GREATER, ">"              },
                { IDMEFV2_CRITERION_OPERATOR_LESSER_OR_EQUAL, "<="     },
                { IDMEFV2_CRITERION_OPERATOR_GREATER_OR_EQUAL, ">="    },

                { IDMEFV2_CRITERION_OPERATOR_REGEX, "~"                },
                { IDMEFV2_CRITERION_OPERATOR_REGEX_NOCASE, "~*"        },
                { IDMEFV2_CRITERION_OPERATOR_NOT_REGEX, "!~"           },
                { IDMEFV2_CRITERION_OPERATOR_NOT_REGEX_NOCASE, "!~*"   },

                { IDMEFV2_CRITERION_OPERATOR_SUBSTR, "<>"              },
                { IDMEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE, "<>*"      },
                { IDMEFV2_CRITERION_OPERATOR_NOT_SUBSTR, "!<>"         },
                { IDMEFV2_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE, "!<>*" },

                { IDMEFV2_CRITERION_OPERATOR_NOT_NULL, ""              },
                { IDMEFV2_CRITERION_OPERATOR_NULL, "!"                 },
        };

        for ( i = 0; tbl[i].operator != 0; i++ )
                if ( op == tbl[i].operator )
                        return tbl[i].name;

        return NULL;
}



/**
 * idmefv2_criterion_new:
 * @criterion: Address where to store the created #idmefv2_criterion_t object.
 * @path: Pointer to an #idmefv2_path_t object.
 * @value: Pointer to an #idmefv2_criterion_value_t object.
 * @op: #idmefv2_criterion_operator_t to use for matching this criterion.
 *
 * Creates a new #idmefv2_criterion_t object and store it in @criterion.
 * Matching this criterion will result in comparing the object value
 * pointed by @path against the provided @value, using @op.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_criterion_new(idmefv2_criterion_t **criterion, idmefv2_path_t *path,
                        idmefv2_criterion_value_t *value, idmefv2_criterion_operator_t op)
{
        libidmefv2_return_val_if_fail(path != NULL, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(! (value == NULL && ! (op & IDMEFV2_CRITERION_OPERATOR_NULL)), libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        *criterion = calloc(1, sizeof(**criterion));
        if ( ! *criterion )
                return libidmefv2_error_from_errno(errno);

        (*criterion)->path = path;
        (*criterion)->value = value;
        (*criterion)->operator = op;

        return 0;
}



/**
 * idmefv2_criterion_destroy:
 * @criterion: Pointer to a #idmefv2_criterion_t object.
 *
 * Destroys @criterion and its content.
 */
void idmefv2_criterion_destroy(idmefv2_criterion_t *criterion)
{
        libidmefv2_return_if_fail(criterion);

        idmefv2_path_destroy(criterion->path);

        if ( criterion->value ) /* can be NULL if operator is is_null or is_not_null */
                idmefv2_criterion_value_destroy(criterion->value);

        free(criterion);
}



/**
 * idmefv2_criterion_clone:
 * @criterion: Pointer to a #idmefv2_criterion_t object to clone.
 * @dst: Address where to store the cloned #idmefv2_criterion_t object.
 *
 * Clones @criterion and stores the cloned criterion within @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_criterion_clone(const idmefv2_criterion_t *criterion, idmefv2_criterion_t **dst)
{
        int ret;
        idmefv2_path_t *path;
        idmefv2_criterion_value_t *value = NULL;

        libidmefv2_return_val_if_fail(criterion, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_path_clone(criterion->path, &path);
        if ( ret < 0 )
                return ret;

        if ( criterion->value ) {
                ret = idmefv2_criterion_value_clone(criterion->value, &value);
                if ( ret < 0 ) {
                        idmefv2_path_destroy(path);
                        return ret;
                }
        }

        ret = idmefv2_criterion_new(dst, path, value, criterion->operator);
        if ( ret < 0 ) {
                idmefv2_path_destroy(path);
                idmefv2_criterion_value_destroy(value);
                return ret;
        }

        return 0;
}



/**
 * idmefv2_criterion_print:
 * @criterion: Pointer to a #idmefv2_criterion_t object.
 * @fd: Pointer to a #libidmefv2_io_t object.
 *
 * Dump @criterion to @fd in the form of:
 * [path] [operator] [value]
 *
 * Or if there is no value associated with the criterion:
 * [operator] [path]
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_criterion_print(const idmefv2_criterion_t *criterion, libidmefv2_io_t *fd)
{
        int ret;
        libidmefv2_string_t *out;

        libidmefv2_return_val_if_fail(criterion, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(fd, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = libidmefv2_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_criterion_to_string(criterion, out);
        if ( ret < 0 ) {
                libidmefv2_string_destroy(out);
                return ret;
        }

        ret = libidmefv2_io_write(fd, libidmefv2_string_get_string(out), libidmefv2_string_get_len(out));
        libidmefv2_string_destroy(out);

        return ret;
}



/**
 * idmefv2_criterion_to_string:
 * @criterion: Pointer to a #idmefv2_criterion_t object.
 * @out: Pointer to a #libidmefv2_string_t object.
 *
 * Dump @criterion as a string to the @out buffer in the form of:
 * [path] [operator] [value]
 *
 * Or if there is no value associated with the criterion:
 * [operator] [path]
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_criterion_to_string(const idmefv2_criterion_t *criterion, libidmefv2_string_t *out)
{
        const char *name, *operator;

        libidmefv2_return_val_if_fail(criterion, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(out, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        operator = idmefv2_criterion_operator_to_string(criterion->operator);
        if ( ! operator )
                return -1;

        name = idmefv2_path_get_name(criterion->path, -1);

        if ( ! criterion->value )
                return libidmefv2_string_sprintf(out, "%s%s%s", operator, (*operator) ? " " : "", name);

        libidmefv2_string_sprintf(out, "%s %s ", name, operator);

        return idmefv2_criterion_value_to_string(criterion->value, out);
}



/**
 * idmefv2_criterion_get_path:
 * @criterion: Pointer to a #idmefv2_criterion_t object.
 *
 * Used to access the #idmefv2_path_t object associated with @criterion.
 *
 * Returns: the #idmefv2_path_t object associated with @criterion.
 */
idmefv2_path_t *idmefv2_criterion_get_path(const idmefv2_criterion_t *criterion)
{
        libidmefv2_return_val_if_fail(criterion, NULL);
        return criterion->path;
}



/**
 * idmefv2_criterion_get_value:
 * @criterion: Pointer to a #idmefv2_criterion_t object.
 *
 * Used to access the #idmefv2_criterion_value_t associated with @criterion.
 * There might be no value specifically if the provided #idmefv2_criterion_operator_t
 * was IDMEFV2_CRITERION_OPERATOR_NULL or IDMEFV2_CRITERION_OPERATOR_NOT_NULL.
 *
 * Returns: the #idmefv2_criterion_value_t object associated with @criterion.
 */
idmefv2_criterion_value_t *idmefv2_criterion_get_value(const idmefv2_criterion_t *criterion)
{
        libidmefv2_return_val_if_fail(criterion, NULL);
        return criterion->value;
}




/**
 * idmefv2_criterion_get_operator:
 * @criterion: Pointer to a #idmefv2_criterion_t object.
 *
 * Used to access the #idmefv2_criterion_operator_t enumeration associated with @criterion.
 *
 * Returns: the #idmefv2_criterion_operator_t associated with @criterion.
 */
idmefv2_criterion_operator_t idmefv2_criterion_get_operator(const idmefv2_criterion_t *criterion)
{
        libidmefv2_return_val_if_fail(criterion, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return criterion->operator;
}



/**
 * idmefv2_criterion_match:
 * @criterion: Pointer to a #idmefv2_criterion_t object.
 * @object: Pointer to a #idmefv2_object_t object to match against @criterion.
 *
 * Matches @message against the provided @criterion. This implies retrieving the
 * value associated with @criterion path, and matching it with the @idmefv2_criterion_value_t
 * object within @criterion.
 *
 * Returns: 1 for a match, 0 for no match, or a negative value if an error occured.
 */
int idmefv2_criterion_match(const idmefv2_criterion_t *criterion, void *object)
{
        int ret;
        idmefv2_value_t *value = NULL;

        libidmefv2_return_val_if_fail(criterion, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(object, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_path_get(criterion->path, object, &value);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_criterion_value_match(criterion->value, value, criterion->operator);
        if ( value )
                idmefv2_value_destroy(value);

        if ( ret < 0 )
                return ret;

        return (ret > 0) ? 1 : 0;
}



/**
 * idmefv2_criteria_new:
 * @criteria: Address where to store the created #idmefv2_criteria_t object.
 *
 * Creates a new #idmefv2_criteria_t object and store it into @criteria.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_criteria_new(idmefv2_criteria_t **criteria)
{
        *criteria = calloc(1, sizeof(**criteria));
        if ( ! *criteria )
                return libidmefv2_error_from_errno(errno);

        (*criteria)->or = NULL;
        (*criteria)->and = NULL;
        (*criteria)->refcount = 1;

        return 0;
}



/**
 * idmefv2_criteria_destroy:
 * @criteria: Pointer to a #idmefv2_criteria_t object.
 *
 * Destroys @criteria and its content.
 */
void idmefv2_criteria_destroy(idmefv2_criteria_t *criteria)
{
        libidmefv2_return_if_fail(criteria);

        if ( --criteria->refcount )
                return;

        if ( criteria->criterion )
                idmefv2_criterion_destroy(criteria->criterion);

        if ( criteria->or )
                idmefv2_criteria_destroy(criteria->or);

        if ( criteria->and )
                idmefv2_criteria_destroy(criteria->and);

        free(criteria);
}



/**
 * idmefv2_criteria_ref:
 * @criteria: Pointer to a #idmefv2_criteria_t object to reference.
 *
 * Increases @criteria reference count.
 *
 * idmefv2_criteria_destroy() will decrease the refcount until it reaches
 * 0, at which point @criteria will be destroyed.
 *
 * Returns: @criteria.
 */
idmefv2_criteria_t *idmefv2_criteria_ref(idmefv2_criteria_t *criteria)
{
        libidmefv2_return_val_if_fail(criteria, NULL);

        criteria->refcount++;
        return criteria;
}


/**
 * idmefv2_criteria_clone:
 * @src: Pointer to a #idmefv2_criteria_t object to clone.
 * @dst: Address where to store the cloned #idmefv2_criteria_t object.
 *
 * Clones @src and stores the cloned criteria within @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_criteria_clone(idmefv2_criteria_t *src, idmefv2_criteria_t **dst)
{
        int ret;
        idmefv2_criteria_t *new;

        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_criteria_new(dst);
        if ( ret < 0 )
                return ret;

        new = *dst;
        new->negated = src->negated;

        if ( src->or ) {
                ret = idmefv2_criteria_clone(src->or, &new->or);
                if ( ret < 0 ) {
                        idmefv2_criteria_destroy(new);
                        return ret;
                }
        }

        if ( src->and ) {
                ret = idmefv2_criteria_clone(src->and, &new->and);
                if ( ret < 0 ) {
                        idmefv2_criteria_destroy(new);
                        return ret;
                }
        }

        ret = idmefv2_criterion_clone(src->criterion, &new->criterion);
        if ( ret < 0 ) {
                idmefv2_criteria_destroy(new);
                return ret;
        }

        return 0;
}



idmefv2_criteria_t *idmefv2_criteria_get_or(const idmefv2_criteria_t *criteria)
{
        libidmefv2_return_val_if_fail(criteria, NULL);
        return criteria->or;
}



idmefv2_criteria_t *idmefv2_criteria_get_and(const idmefv2_criteria_t *criteria)
{
        libidmefv2_return_val_if_fail(criteria, NULL);
        return criteria->and;
}



int idmefv2_criteria_print(const idmefv2_criteria_t *criteria, libidmefv2_io_t *fd)
{
        int ret;
        libidmefv2_string_t *out;

        libidmefv2_return_val_if_fail(criteria, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(fd, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = libidmefv2_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_criteria_to_string(criteria, out);
        if ( ret < 0 )
                return ret;

        ret = libidmefv2_io_write(fd, libidmefv2_string_get_string(out), libidmefv2_string_get_len(out));
        libidmefv2_string_destroy(out);

        return ret;
}



int idmefv2_criteria_to_string(const idmefv2_criteria_t *criteria, libidmefv2_string_t *out)
{
        libidmefv2_return_val_if_fail(criteria, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(out, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( criteria->or )
                libidmefv2_string_sprintf(out, "((");

        idmefv2_criterion_to_string(criteria->criterion, out);

        if ( criteria->and ) {
                libidmefv2_string_sprintf(out, " && ");
                idmefv2_criteria_to_string(criteria->and, out);
        }

        if ( criteria->or ) {
                libidmefv2_string_sprintf(out, ") || (");
                idmefv2_criteria_to_string(criteria->or, out);
                libidmefv2_string_sprintf(out, "))");
        }

        return 0;
}



libidmefv2_bool_t idmefv2_criteria_is_criterion(const idmefv2_criteria_t *criteria)
{
        libidmefv2_return_val_if_fail(criteria, FALSE);
        return (criteria->criterion != NULL) ? TRUE : FALSE;
}



idmefv2_criterion_t *idmefv2_criteria_get_criterion(const idmefv2_criteria_t *criteria)
{
        libidmefv2_return_val_if_fail(criteria, NULL);
        return criteria->criterion;
}



void idmefv2_criteria_or_criteria(idmefv2_criteria_t *criteria, idmefv2_criteria_t *criteria2)
{
        libidmefv2_return_if_fail(criteria);
        libidmefv2_return_if_fail(criteria2);

        while ( criteria->or )
                criteria = criteria->or;

        criteria->or = criteria2;
}



int idmefv2_criteria_and_criteria(idmefv2_criteria_t *criteria, idmefv2_criteria_t *criteria2)
{
        int ret;
        idmefv2_criteria_t *new, *last = NULL;

        libidmefv2_return_val_if_fail(criteria, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(criteria2, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        while ( criteria ) {
                last = criteria;

                if ( criteria->or ) {
                        ret = idmefv2_criteria_clone(criteria2, &new);
                        if ( ret < 0 )
                                return ret;

                        ret = idmefv2_criteria_and_criteria(criteria->or, new);
                        if ( ret < 0 )
                                return ret;
                }

                criteria = criteria->and;
        }

        last->and = criteria2;

        return 0;
}


void idmefv2_criteria_set_negation(idmefv2_criteria_t *criteria, libidmefv2_bool_t negate)
{
        libidmefv2_return_if_fail(criteria);
        criteria->negated = negate;
}


libidmefv2_bool_t idmefv2_criteria_get_negation(const idmefv2_criteria_t *criteria)
{
        libidmefv2_return_val_if_fail(criteria, FALSE);
        return criteria->negated;
}


void idmefv2_criteria_set_criterion(idmefv2_criteria_t *criteria, idmefv2_criterion_t *criterion)
{
        libidmefv2_return_if_fail(criteria);
        libidmefv2_return_if_fail(criterion);

        criteria->criterion = criterion;
}




/**
 * idmefv2_criteria_match:
 * @criteria: Pointer to a #idmefv2_criteria_t object.
 * @object: Pointer to a #idmefv2_object_t object.
 *
 * Matches @object against the provided criteria.
 *
 * Returns: 1 if criteria match, 0 if it did not, a negative value if an error occured.
 */
int idmefv2_criteria_match(const idmefv2_criteria_t *criteria, void *object)
{
        int ret;

        libidmefv2_return_val_if_fail(criteria, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(object, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_criterion_match(criteria->criterion, object);
        if ( ret < 0 )
                return ret;

        if ( ret == 1 && criteria->and )
                ret = idmefv2_criteria_match(criteria->and, object);

        if ( ret == 0 && criteria->or )
                ret = idmefv2_criteria_match(criteria->or, object);

        if ( ret < 0 )
                return ret;

        return (criteria->negated) ? !ret : ret;
}

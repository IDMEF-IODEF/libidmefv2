/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libidmefv2-ids.com>
* Author: Krzysztof Zaraska
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>

#include "libmissing.h"
#include "libidmefv2-list.h"
#include "libidmefv2-log.h"
#include "libidmefv2-inttypes.h"
#include "libidmefv2-string.h"

#define LIBIDMEFV2_ERROR_SOURCE_DEFAULT LIBIDMEFV2_ERROR_SOURCE_IDMEFV2_TYPE
#include "libidmefv2-error.h"

#include "idmefv2-time.h"
#include "idmefv2-data.h"
#include "idmefv2-value.h"
#include "idmefv2-class.h"

#include "idmefv2-tree-wrap.h"
#include "idmefv2-message-print.h"
#include "idmefv2-message-print-json.h"
#include "idmefv2-message-write.h"
#include "idmefv2-tree-data.h"
#include "idmefv2-path.h"



static inline int is_class_valid(idmefv2_class_id_t class)
{
        if ( class < 0 || (size_t) class >= sizeof(object_data) / sizeof(*object_data) )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_IDMEFV2_CLASS_UNKNOWN, "Unknown IDMEFV2 class '%d'", (int) class);

        return 0;
}


static inline int is_child_valid(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;
        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        if ( child < 0 || (size_t) child >= object_data[class].children_list_elem )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_IDMEFV2_CLASS_UNKNOWN_CHILD, "Unknown IDMEFV2 child '%d' for class '%s'",
                                             (int) child, object_data[class].name);

        return 0;
}




idmefv2_class_child_id_t idmefv2_class_find_child(idmefv2_class_id_t class, const char *name)
{
        int ret;
        size_t i;
        const children_list_t *list;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        list = object_data[class].children_list;

        if ( list ) {
                for ( i = 0; i < object_data[class].children_list_elem; i++ )
                        if ( strcasecmp(list[i].name, name) == 0)
                                return i;
        }

        return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_IDMEFV2_CLASS_UNKNOWN_CHILD, "Unknown IDMEFV2 child '%s' for class '%s'", name, idmefv2_class_get_name(class));
}




libidmefv2_bool_t idmefv2_class_is_child_union_member(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;
        
        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return (object_data[class].children_list[child].union_id > 0) ? TRUE : FALSE;
}



libidmefv2_bool_t idmefv2_class_is_child_list(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].list;
}



libidmefv2_bool_t idmefv2_class_is_child_keyed_list(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].keyed_list;
}



idmefv2_value_type_id_t idmefv2_class_get_child_value_type(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].type;
}



size_t idmefv2_class_get_child_count(idmefv2_class_id_t class)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list_elem;
}



int idmefv2_class_get_child_union_id(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;
        const children_list_t *c;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        c = &object_data[class].children_list[child];
        if ( ! c->union_id )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_CLASS_CHILD_NOT_UNION);

        return c->union_id;
}



idmefv2_class_id_t idmefv2_class_get_child_class(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;
        const children_list_t *c;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        c = &object_data[class].children_list[child];
        if ( c->type != IDMEFV2_VALUE_TYPE_CLASS && c->type != IDMEFV2_VALUE_TYPE_ENUM )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_CLASS_CHILD_NOT_CLASS);

        return c->class;
}



const char *idmefv2_class_get_child_name(idmefv2_class_id_t class, idmefv2_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return NULL;

        return object_data[class].children_list[child].name;
}




idmefv2_class_id_t idmefv2_class_find(const char *name)
{
        idmefv2_class_id_t i;

        for ( i = 0; object_data[i].name != NULL; i++ )
                if ( strcasecmp(object_data[i].name, name) == 0 )
                        return i;

        return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_IDMEFV2_CLASS_UNKNOWN_NAME, "Unknown IDMEFV2 class '%s'", name);
}


int idmefv2_class_enum_to_numeric(idmefv2_class_id_t class, const char *val)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        if ( ! object_data[class].to_numeric )
                return -1;

            return object_data[class].to_numeric(val);
}


const char *idmefv2_class_enum_to_string(idmefv2_class_id_t class, int val)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return NULL;

        if ( ! object_data[class].to_string )
                return NULL;

        return object_data[class].to_string(val);
}


int idmefv2_class_get_child(void *ptr, idmefv2_class_id_t class, idmefv2_class_child_id_t child, void **childptr)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].get_child(ptr, child, childptr);
}




int idmefv2_class_new_child(void *ptr, idmefv2_class_id_t class, idmefv2_class_child_id_t child, int n, void **childptr)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].new_child(ptr, child, n, childptr);
}



int idmefv2_class_destroy_child(void *ptr, idmefv2_class_id_t class, idmefv2_class_child_id_t child, int n)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].destroy_child(ptr, child, n);
}


int idmefv2_class_copy(idmefv2_class_id_t class, const void *src, void *dst)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].copy(src, dst);
}



int idmefv2_class_clone(idmefv2_class_id_t class, const void *src, void **dst)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].clone(src, dst);
}



int idmefv2_class_compare(idmefv2_class_id_t class, const void *c1, const void *c2)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].compare(c1, c2);
}



int idmefv2_class_destroy(idmefv2_class_id_t class, void *obj)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        object_data[class].destroy(obj);

        return 0;
}



int idmefv2_class_ref(idmefv2_class_id_t class, void *obj)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        object_data[class].ref(obj);

        return 0;
}



int idmefv2_class_print(idmefv2_class_id_t class, void *obj, libidmefv2_io_t *fd)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print(obj, fd);
}


int idmefv2_class_print_json(idmefv2_class_id_t class, void *obj, libidmefv2_io_t *fd)
{
        int ret;
        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print_json(obj, fd);
}


int idmefv2_class_print_binary(idmefv2_class_id_t class, void *obj, libidmefv2_io_t *fd)
{
        int ret;
        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print_binary(obj, fd);
}


const char *idmefv2_class_get_name(idmefv2_class_id_t class)
{
        if ( is_class_valid(class) < 0 )
                return NULL;

        return object_data[class].name;
}



libidmefv2_bool_t idmefv2_class_is_listed(idmefv2_class_id_t class)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].is_listed;
}

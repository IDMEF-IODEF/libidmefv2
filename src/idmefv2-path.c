/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Krzysztof Zaraska
*         Yoann Vandoorselaere <yoann@libidmefv2-ids.com>
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

#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>
#include <limits.h>
#include <assert.h>

#include "glthread/lock.h"

#include "libidmefv2-hash.h"
#include "libidmefv2-inttypes.h"
#include "libidmefv2-string.h"

#define LIBIDMEFV2_ERROR_SOURCE_DEFAULT LIBIDMEFV2_ERROR_SOURCE_IDMEFV2_PATH
#include "libidmefv2-error.h"

#include "idmefv2-time.h"
#include "idmefv2-data.h"
#include "idmefv2-class.h"
#include "idmefv2-value.h"

#include "idmefv2-object-prv.h"
#include "idmefv2-tree-wrap.h"
#include "idmefv2-path.h"
#include "libidmefv2-string.h"
#include "libidmefv2-linked-object.h"
#include "common.h"


#define MAX_DEPTH                16
#define MAX_NAME_LEN            128
#define HASH_DEFAULT_SIZE       128

#define INDEX_UNDEFINED INT_MIN
#define INDEX_KEY       (INT_MIN + 1)
#define INDEX_FORBIDDEN (INT_MIN + 2)


typedef struct idmefv2_key_listed_object {
        IDMEFV2_OBJECT;
        libidmefv2_list_t list;
        libidmefv2_string_t *objkey;
} idmefv2_key_listed_object_t;


typedef struct idmefv2_path_element {

        int index;
        char *index_key;

        idmefv2_class_id_t class;
        idmefv2_class_child_id_t position;
        idmefv2_value_type_id_t value_type;

} idmefv2_path_element_t;


struct idmefv2_path {

        gl_lock_t mutex;
        char name[MAX_NAME_LEN];
        int refcount;
        unsigned int depth;
        idmefv2_class_id_t top_class;

        idmefv2_path_element_t elem[MAX_DEPTH];
};



static int _idmefv2_path_set(const idmefv2_path_t *path, idmefv2_class_id_t class, size_t i,
                           int index_override, void *ptr, idmefv2_value_t *value);


static libidmefv2_bool_t flush_cache = FALSE;
static libidmefv2_hash_t *cached_path = NULL;
static gl_lock_t cached_path_mutex = gl_lock_initializer;



static void path_lock_cb(void *data)
{
        idmefv2_path_t *path = data;
        gl_lock_lock(path->mutex);
}


static void path_reinit_cb(void *data)
{
        idmefv2_path_t *path = data;
        gl_lock_init(path->mutex);
}


static void path_unlock_cb(void *data)
{
        idmefv2_path_t *path = data;
        gl_lock_unlock(path->mutex);
}



static void flush_cache_if_wanted(void *ptr)
{
        if ( flush_cache )
                idmefv2_path_destroy(ptr);
}



static unsigned int path_hash_func(const void *path)
{
        const char *ptr = ((const idmefv2_path_t *) path)->name;
        unsigned int hv = *ptr;

        if ( hv )
                for ( ptr += 1; *ptr; ptr++ )
                        hv = (hv << 5) - hv + *ptr;

        return hv;

}


static int path_key_cmp_func(const void *p1, const void *p2)
{
        const idmefv2_path_t *path1 = p1;
        const idmefv2_path_t *path2 = p2;

        if ( path1->top_class != path2->top_class )
                return -1;

        return strcmp(path1->name, path2->name);
}



static int initialize_path_cache_if_needed(void)
{
        if ( cached_path )
                return 0;

        return libidmefv2_hash_new2(&cached_path, HASH_DEFAULT_SIZE, path_hash_func, path_key_cmp_func, NULL, flush_cache_if_wanted);
}



static int build_name(idmefv2_path_t *path)
{
        unsigned int i;
        const char *name;
        idmefv2_class_id_t class;

        /*
         * we don't need gl_lock_{,un}lock since the path has no name
         * it means that it is not in the cache and thus, not shared
         */
        path->name[0] = '\0';
        class = IDMEFV2_CLASS_ID_MESSAGE;

        for ( i = 0; i < path->depth; i++ ) {

                if ( i > 0 )
                        strncat(path->name, ".", sizeof(path->name) - strlen(path->name));

                name = idmefv2_path_get_name(path, i);
                if ( ! name )
                        return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INTEGRITY);

                strncat(path->name, name, sizeof(path->name) - strlen(path->name));

                if ( path->elem[i].index != INDEX_UNDEFINED && path->elem[i].index != INDEX_FORBIDDEN ) {
                        strncat(path->name, "(", sizeof(path->name) - strlen(path->name));

                        if ( path->elem[i].index != INDEX_KEY )
                                snprintf(path->name + strlen(path->name), sizeof(path->name) - strlen(path->name), "%d", path->elem[i].index);

                        else if ( path->elem[i].index_key )
                                strncat(path->name, path->elem[i].index_key, sizeof(path->name) - strlen(path->name));

                        strncat(path->name, ")", sizeof(path->name) - strlen(path->name));
                }

                class = idmefv2_class_get_child_class(class, path->elem[i].position);
                if ( class < 0 && i < path->depth - 1 )
                        return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INTEGRITY);
        }

        return 0;
}




static int idmefv2_path_get_internal(idmefv2_value_t **ret, const idmefv2_path_t *path,
                                   unsigned int depth, void *parent, idmefv2_class_id_t parent_class);


static inline libidmefv2_bool_t has_index_key(libidmefv2_list_t *elem, const char *index_key)
{
        idmefv2_key_listed_object_t *obj = libidmefv2_list_entry(elem, idmefv2_key_listed_object_t, list);
        return (obj->objkey && strcmp(libidmefv2_string_get_string_or_default(obj->objkey, ""), index_key) == 0) ? TRUE : FALSE;
}


static int set_index_key(const idmefv2_path_element_t *elem, void *ptr)
{
        idmefv2_key_listed_object_t *obj = ptr;

        if ( ! elem->index_key )
                return 0;

        if ( obj->objkey )
                libidmefv2_string_destroy(obj->objkey);

        return libidmefv2_string_new_dup(&obj->objkey, elem->index_key);
}



static int idmefv2_path_get_list_internal(idmefv2_value_t **value_list,
                                        const idmefv2_path_t *path, int depth,
                                        libidmefv2_list_t *list, idmefv2_class_id_t parent_class)
{
        int ret;
        libidmefv2_list_t *tmp;
        idmefv2_value_t *value;
        unsigned int cnt = 0;

        ret = idmefv2_value_new_list(value_list);
        if ( ret < 0 )
                return ret;

        libidmefv2_list_for_each(list, tmp) {
                value = NULL;

                if ( path->elem[depth - 1].index_key && ! has_index_key(tmp, path->elem[depth - 1].index_key) )
                        continue;

                if ( parent_class >= 0 )
                        ret = idmefv2_path_get_internal(&value, path, depth, idmefv2_linked_object_get_object(tmp), parent_class);
                else {
                        idmefv2_value_type_id_t type = path->elem[depth - 1].value_type;

                        ret = idmefv2_value_new(&value, type, idmefv2_linked_object_get_object(tmp));
                        if ( ret == 0 ) {
                                idmefv2_value_type_t vt;

                                vt.id = type;
                                vt.data.string_val = (void *) idmefv2_linked_object_get_object(tmp);

                                ret = idmefv2_value_type_ref(&vt);
                                if ( ret < 0 )
                                        idmefv2_value_destroy(value);
                        }
                }

                if ( ret < 0 ) {
                        idmefv2_value_destroy(*value_list);
                        return ret;
                }

                ret = idmefv2_value_list_add(*value_list, value);
                if ( ret < 0 ) {
                        idmefv2_value_destroy(*value_list);
                        return ret;
                }

                cnt++;
        }

        if ( ! cnt ) {
                idmefv2_value_destroy(*value_list);
                *value_list = NULL;
        }

        return cnt;
}


static int idmefv2_path_get_nth_internal(idmefv2_value_t **value, const idmefv2_path_t *path,
                                       unsigned int depth, libidmefv2_list_t *list,
                                       idmefv2_class_id_t parent_class, int which, const char *index_key)
{
        int cnt = 0;
        libidmefv2_list_t *tmp = NULL;

        if ( which >= 0 ) {
                libidmefv2_list_for_each(list, tmp) {
                        if ( cnt++ == which )
                                return idmefv2_path_get_internal(value, path, depth, idmefv2_linked_object_get_object(tmp), parent_class);
                }
        } else {
                which = -which;
                which--; /* With negative value, -1 is the base, translate to 0 */

                libidmefv2_list_for_each_reversed(list, tmp) {
                        if ( cnt++ == which )
                                return idmefv2_path_get_internal(value, path, depth, idmefv2_linked_object_get_object(tmp), parent_class);
                }
        }

        return 0;
}



static int idmefv2_path_get_internal(idmefv2_value_t **value, const idmefv2_path_t *path,
                                   unsigned int depth, void *parent, idmefv2_class_id_t parent_class)
{
        void *child;
        int ret, which;
        idmefv2_class_id_t child_class;
        idmefv2_class_child_id_t child_id;

        if ( depth < path->depth ) {

                child_id = path->elem[depth].position;

                ret = idmefv2_class_get_child(parent, parent_class, child_id, &child);
                if ( ret < 0 )
                        return ret;

                if ( ! child ) {
                        *value = NULL;
                        return 0;
                }

                child_class = idmefv2_class_get_child_class(parent_class, child_id);
                which = path->elem[depth].index;

                if ( which == INDEX_FORBIDDEN )
                        return idmefv2_path_get_internal(value, path, depth + 1, child, child_class);

                if ( which == INDEX_UNDEFINED || which == INDEX_KEY || which == IDMEFV2_LIST_APPEND || which == IDMEFV2_LIST_PREPEND )
                        return idmefv2_path_get_list_internal(value, path, depth + 1, child, child_class);

                return idmefv2_path_get_nth_internal(value, path, depth + 1, child, child_class, which, path->elem[depth].index_key);
        }

        if ( parent_class < 0 || (path->depth > 0 && path->elem[path->depth - 1].value_type == IDMEFV2_VALUE_TYPE_ENUM) ) {
                *value = parent;
                return 1;
        }

        ret = (ret = idmefv2_value_new_class(value, parent_class, parent) < 0) ? ret : 1;
        if ( ret == 1 )
                idmefv2_class_ref(parent_class, parent);

        return ret;
}


static void delete_listed_child(void *parent, idmefv2_class_id_t class, const idmefv2_path_element_t *elem)
{
        int ret;
        void *obj;
        libidmefv2_list_t *head, *tmp, *bkp;

        ret = idmefv2_class_get_child(parent, class, elem->position, (void *) &head);
        if ( ret < 0 )
                return;

        libidmefv2_list_for_each_safe(head, tmp, bkp) {
                obj = idmefv2_linked_object_get_object(tmp);

                if ( elem->index_key && ! has_index_key(obj, elem->index_key) )
                        continue;

                /*
                 * The object might be referenced from other place than
                 * this message, in which case idmefv2_class_destroy()
                 * will only decrease it's reference count.
                 *
                 * We manually call libidmefv2_list_del_init() in order to
                 * disassociate the object from the message.
                 */
                libidmefv2_list_del_init(tmp);
                idmefv2_class_destroy(idmefv2_class_get_child_class(class, elem->position), obj);
        }
}



static int _idmefv2_path_set_undefined_not_last(const idmefv2_path_t *path, const idmefv2_path_element_t *elem,
                                              idmefv2_class_id_t class, size_t i, void *ptr, idmefv2_value_t *value)
{
        int j = 0, ret;
        idmefv2_value_t *val;
        libidmefv2_list_t *head, *tmp, *bkp;

        assert(idmefv2_class_is_child_list(class, elem->position));

        ret = idmefv2_class_get_child(ptr, class, elem->position, (void *) &head);
        if ( ret < 0 )
                return ret;

        libidmefv2_list_for_each_safe(head, tmp, bkp) {
                if ( elem->index_key && ! has_index_key(tmp, elem->index_key) )
                        continue;

                if ( value && idmefv2_value_is_list(value) ) {
                        ret = idmefv2_value_get_nth2(value, j++, &val);
                        if ( ret <= 0 )
                                return ret;
                } else {
                        j = 1;
                        val = value;
                }

                ret = _idmefv2_path_set(path, elem->class, i + 1, 0, idmefv2_linked_object_get_object(tmp), val);
                if ( ret < 0 )
                        return ret;
        }

        for ( ; value && j < idmefv2_value_get_count(value); j++ ) {
                ret = _idmefv2_path_set(path, class, i, IDMEFV2_LIST_APPEND, ptr, idmefv2_value_get_nth(value, j));
                if ( ret < 0 )
                        return ret;

                ret = set_index_key(elem, idmefv2_linked_object_get_object(head->prev));
                if ( ret < 0 )
                        return ret;
        }

        return ret;
}



static void ad_magic_set_datatype(idmefv2_additional_data_t *data, idmefv2_value_type_id_t vtype)
{
        idmefv2_additional_data_type_t type;

        /*
         * In case the user has already set the type member, do nothing.
         */
        if ( _idmefv2_additional_data_type_is_set(data) != 0 )
                return;

        if ( vtype >= IDMEFV2_VALUE_TYPE_INT8 && vtype <= IDMEFV2_VALUE_TYPE_UINT64 )
                type = IDMEFV2_ADDITIONAL_DATA_TYPE_INTEGER;

        else if ( vtype == IDMEFV2_VALUE_TYPE_FLOAT || vtype == IDMEFV2_VALUE_TYPE_DOUBLE )
                type = IDMEFV2_ADDITIONAL_DATA_TYPE_REAL;

        else if ( vtype == IDMEFV2_VALUE_TYPE_STRING )
                type = IDMEFV2_ADDITIONAL_DATA_TYPE_STRING;

        else if ( vtype == IDMEFV2_VALUE_TYPE_DATA )
                type = IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING;

       else if ( vtype == IDMEFV2_VALUE_TYPE_TIME )
                type = IDMEFV2_ADDITIONAL_DATA_TYPE_DATE_TIME;

        else
                return;

        idmefv2_additional_data_set_type(data, type);
}



static int _idmefv2_path_set(const idmefv2_path_t *path, idmefv2_class_id_t class, size_t i,
                           int index_override, void *ptr, idmefv2_value_t *value)
{
        void *prevptr = NULL;
        int ret, index, j;
        libidmefv2_bool_t is_last_element;
        idmefv2_value_type_id_t tid;
        const idmefv2_path_element_t *elem;
        idmefv2_class_id_t parent_class;

        parent_class = (class == IDMEFV2_CLASS_ID_MESSAGE) ? class : path->elem[i - 1].class;

        for ( ; i < path->depth; i++ ) {

                elem = &path->elem[i];
                index = (index_override) ? index_override : elem->index;
                index_override = 0;
                is_last_element = (i == (path->depth - 1));

                if ( index == INDEX_UNDEFINED || index == INDEX_KEY ) {
                        if ( ! is_last_element )
                                return _idmefv2_path_set_undefined_not_last(path, elem, class, i, ptr, value);
                        else {
                                delete_listed_child(ptr, class, elem);

                                for ( j = 0; value && j < idmefv2_value_get_count(value); j++ ) {
                                        ret = _idmefv2_path_set(path, class, i, IDMEFV2_LIST_APPEND, ptr, idmefv2_value_get_nth(value, j));
                                        if ( ret < 0 )
                                                return ret;

                                        if ( index == INDEX_KEY ) {
                                                libidmefv2_list_t *head;

                                                ret = idmefv2_class_get_child(ptr, class, elem->position, (void *) &head);
                                                if ( ret < 0 )
                                                        return ret;

                                                ret = set_index_key(elem, head->prev);
                                                if ( ret < 0 )
                                                        return ret;
                                        }
                                }

                                return 0;
                        }
                }

                parent_class = class;

                if ( value || ! is_last_element ) {
                        prevptr = ptr;

                        ret = idmefv2_class_new_child(ptr, class, elem->position, index, &ptr);
                        if ( ret < 0 )
                                return ret;

                        class = idmefv2_class_get_child_class(class, elem->position);
                        assert( ! (class < 0 && i < path->depth - 1) );
                }
        }

        elem = &path->elem[path->depth - 1];
        if ( ! value )
                return idmefv2_class_destroy_child(ptr, parent_class, elem->position, elem->index);

        tid = idmefv2_class_get_child_value_type(parent_class, elem->position);
        if ( parent_class == IDMEFV2_CLASS_ID_ADDITIONAL_DATA && tid == IDMEFV2_VALUE_TYPE_DATA && prevptr )
                ad_magic_set_datatype(prevptr, idmefv2_value_get_type(value));

        return _idmefv2_value_copy_internal(value, tid, class, ptr);
}



/**
 * idmefv2_path_get:
 * @path: Pointer to a #idmefv2_path_t object.
 * @message: Pointer to a #idmefv2_message_t object.
 * @ret: Address where to store the retrieved #idmefv2_value_t.
 *
 * This function retrieves the value for @path within @message,
 * and stores it into the provided @ret address of type #idmefv2_value_t.
 *
 * Returns: The number of element retrieved, or a negative value if an error occured.
 */
int idmefv2_path_get(const idmefv2_path_t *path, void *obj, idmefv2_value_t **ret)
{
        idmefv2_object_t *object = obj;

        if ( object->_idmefv2_object_id != path->top_class )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "path for object '%s' used with '%s' object root",
                                             idmefv2_class_get_name(path->top_class), idmefv2_class_get_name(object->_idmefv2_object_id));

        return idmefv2_path_get_internal(ret, path, 0, object, object->_idmefv2_object_id);
}


/**
 * idmefv2_path_set:
 * @path: Pointer to a #idmefv2_path_t object.
 * @message: Pointer to a #idmefv2_message_t object.
 * @value: Pointer to a #idmefv2_value_t object.
 *
 * This function sets the provided @value for @path within @message.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_path_set(const idmefv2_path_t *path, void *obj, idmefv2_value_t *value)
{
        idmefv2_object_t *object = obj;

        if ( path->depth < 1 )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "Path with depth of 0 are not allowed");

        if ( object->_idmefv2_object_id != path->top_class )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "path for object '%s' used with '%s' object root",
                                             idmefv2_class_get_name(path->top_class), idmefv2_class_get_name(object->_idmefv2_object_id));


        return _idmefv2_path_set(path, object->_idmefv2_object_id, 0, 0, object, value);
}



static int copy_path_name(idmefv2_path_t *path, const char *buffer)
{
        size_t len;

        len = strlen(buffer) + 1;
        if ( len >= sizeof(path->name) )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_LENGTH);

        memcpy(path->name, buffer, len);

        return 0;
}



/*
 * idmefv2_object_create returns:
 * -1 if something wrong happen
 * 0 for a new empty object
 * 1 for an existing object already in the cache
 */
static int idmefv2_path_create(idmefv2_path_t **path, idmefv2_class_id_t rootclass, const char *buffer)
{
        int ret;
        idmefv2_path_t tmp;

        ret = copy_path_name(&tmp, buffer);
        if ( ret < 0 )
                return ret;

        tmp.top_class = rootclass;

        gl_lock_lock(cached_path_mutex);

        ret = initialize_path_cache_if_needed();
        if ( ret < 0 ) {
                gl_lock_unlock(cached_path_mutex);
                return ret;
        }

        *path = libidmefv2_hash_get(cached_path, &tmp);
        gl_lock_unlock(cached_path_mutex);

        if ( *path )
                return 1;

        *path = calloc(1, sizeof(**path));
        if ( ! *path )
                return libidmefv2_error_from_errno(errno);

        (*path)->refcount = 1;
        gl_lock_init((*path)->mutex);

        return 0;
}



/*
 * return 1 if we are reading the last object.
 * return 0 if reading an object.
 * return -1 if there is no more things to read.
 */
static int parse_path_token(char **sptr, char **out)
{
        char *buf = *sptr, *ptr;

        if ( *buf == '\0' )
                *buf++ = '.';

        *out = buf;

        ptr = strchr(buf, '.');
        if ( ! ptr )
                return -1;

        *ptr = '\0';
        *sptr = ptr;

        return 0;
}



static int idmefv2_path_parse_new(idmefv2_path_t *path, idmefv2_class_id_t rootclass, const char *buffer)
{
        int ret, index = -1, is_last;
        unsigned int depth = 0;
        char *endptr, *ptr, *ptr2, *end;
        idmefv2_class_child_id_t child = 0;
        idmefv2_class_id_t class;
        idmefv2_value_type_id_t vtype;

        ret = copy_path_name(path, buffer);
        if ( ret < 0 )
                return ret;

        end = ptr = NULL;
        endptr = path->name;
        path->top_class = class = rootclass;

        do {
                index = INDEX_UNDEFINED;
                is_last = parse_path_token(&endptr, &ptr);

                ptr2 = strchr(ptr, '(');
                if ( ptr2 ) {
                        char *errptr = NULL;
                        char *tok = ptr2 + 1;

                        end = strchr(ptr2, ')');
                        if ( ! end )
                                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "Malformed IDMEFV2Path index : missing ')' character");

                        *end = *ptr2 = '\0';
                        if ( strcmp(tok, "<<") == 0 )
                                index = IDMEFV2_LIST_PREPEND;

                        else if ( strcmp(tok, ">>") == 0 )
                                index = IDMEFV2_LIST_APPEND;

                        else if ( (*tok == '\'' || *tok == '"') && *(tok + strlen(tok) - 1) == *tok ) {
                                index = INDEX_KEY;
                                path->elem[depth].index_key = strndup(tok + 1, strlen(tok) - 2);
                        }

                        else if ( strcmp(ptr2 + 1, "*") != 0 ) {
                                index = strtol(ptr2 + 1, &errptr, 0);
                                if ( index == 0 && errptr == tok )
                                        return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "Invalid index specified : `%s`", tok);
                        }
                        *end = ')';
                }

                child = idmefv2_class_find_child(class, ptr);
                if ( child < 0 )
                        return child;

                path->elem[depth].position = child;

                if ( ptr2 )
                        *ptr2 = '(';

                if ( index == INDEX_UNDEFINED )
                        path->elem[depth].index = idmefv2_class_is_child_list(class, child) ? INDEX_UNDEFINED : INDEX_FORBIDDEN;
                else {
                        if ( ! idmefv2_class_is_child_list(class, child) )
                                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_FORBIDDEN,
                                                             "Invalid IDMEFV2 path element '%s': indexing not supported", ptr);

                        if ( index == INDEX_KEY && ! idmefv2_class_is_child_keyed_list(class, child) )
                                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_FORBIDDEN,
                                                             "Invalid IDMEFV2 path element '%s': key not supported as index", ptr);

                        path->elem[depth].index = index;
                }

                /* The last object may not be a structure */
                vtype = path->elem[depth].value_type = idmefv2_class_get_child_value_type(class, child);
                if ( vtype != IDMEFV2_VALUE_TYPE_CLASS && ! is_last )
                        return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC,
                                                     "IDMEFV2 element '%s' is a leaf and thus has no child '%s'",
                                                     ptr, endptr + 1);

                class = path->elem[depth].class = idmefv2_class_get_child_class(class, child);
                if ( ++depth == MAX_DEPTH )
                        return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_DEPTH);

        } while ( ! is_last );

        path->depth = depth;

        return 0;
}




/**
 * idmefv2_path_new_from_root_fast:
 * @path: Address where to store the created #idmefv2_path_t object.
 * @rootclass: #idmefv2_class_id_t ID of the root path.
 * @buffer: Name of the path to create.
 *
 * Creates a #idmefv2_path_t object whom root is @rootclass pointing
 * to @buffer, and stores it within @path.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_path_new_from_root_fast(idmefv2_path_t **path, idmefv2_class_id_t rootclass, const char *buffer)
{
        int ret;

        libidmefv2_return_val_if_fail(buffer, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_path_create(path, rootclass, buffer);
        if ( ret < 0 )
                return ret;

        if ( ret == 1 ) {
                idmefv2_path_ref(*path);
                return 0;
        }

        if ( *buffer == '\0' )
                (*path)->top_class = rootclass;
        else {
                ret = idmefv2_path_parse_new(*path, rootclass, buffer);
                if ( ret < 0 ) {
                        gl_lock_destroy((*path)->mutex);
                        free(*path);
                        return ret;
                }
        }

        gl_lock_lock(cached_path_mutex);

        if ( libidmefv2_hash_add(cached_path, *path, *path) < 0 ) {

                gl_lock_destroy((*path)->mutex);
                free(*path);
                gl_lock_unlock(cached_path_mutex);
                return ret;
        }

        gl_lock_unlock(cached_path_mutex);

        idmefv2_path_ref(*path);

        return 0;
}




/**
 * idmefv2_path_new_fast:
 * @path: Address where to store the created #idmefv2_path_t object.
 * @buffer: Name of the path to create.
 *
 * Creates a #idmefv2_path_t object pointing to @buffer, and stores it within @path.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_path_new_fast(idmefv2_path_t **path, const char *buffer)
{
        return idmefv2_path_new_from_root_fast(path, IDMEFV2_CLASS_ID_MESSAGE, buffer);
}



/**
 * idmefv2_path_new_v:
 * @path: Address where to store the created #idmefv2_path_t object.
 * @format: Format string.
 * @args: Pointer to a variable argument list.
 *
 * Creates an #idmefv2_path_t object pointing to the provided format
 * string @format and @args, and stores it within @path.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_path_new_v(idmefv2_path_t **path, const char *format, va_list args)
{
        int ret;
        char buffer[MAX_NAME_LEN];

        libidmefv2_return_val_if_fail(format, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = vsnprintf(buffer, sizeof(buffer), format, args);
        if ( ret < 0 || (size_t) ret > sizeof(buffer) - 1 )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_LENGTH);

        return idmefv2_path_new_fast(path, buffer);
}



/**
 * idmefv2_path_new:
 * @path: Address where to store the created #idmefv2_path_t object.
 * @format: Format string.
 * @...: Arguments list.
 *
 * Creates an #idmefv2_path_t object pointing to the provided format
 * string @format and @..., and stores it within @path.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_path_new(idmefv2_path_t **path, const char *format, ...)
{
        int ret;
        va_list args;

        libidmefv2_return_val_if_fail(format, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        va_start(args, format);
        ret = idmefv2_path_new_v(path, format, args);
        va_end(args);

        return ret;
}



/**
 * idmefv2_path_get_class:
 * @path: Pointer to an #idmefv2_path_t object.
 * @depth: Depth of @path to retrieve the #idmefv2_class_id_t from.
 *
 * Retrieves the #idmefv2_class_id_t value for the element of @path
 * located at @depth. If depth is -1, the last element depth is addressed.
 *
 * Returns: The #idmefv2_class_id_t for the elemnt, or a negative value if an error occured.
 */
idmefv2_class_id_t idmefv2_path_get_class(const idmefv2_path_t *path, int depth)
{
        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( depth < 0 )
                depth = path->depth - 1;

        if ( path->depth == 0 && depth < 0 )
                return path->top_class;

        return path->elem[depth].class;
}



/**
 * idmefv2_path_get_value_type:
 * @path: Pointer to an #idmefv2_path_t object.
 * @depth: Depth of @path to retrieve the #idmefv2_value_type_id_t from.
 *
 * Retrieves the #idmefv2_value_type_id_t identifying the type of value
 * acceptable for this path element, for the @path element located at
 * @depth. If depth is -1, the last element depth is addressed.
 *
 * Returns: The #idmefv2_value_type_id_t for the element, or a negative value if an error occured.
 */
idmefv2_value_type_id_t idmefv2_path_get_value_type(const idmefv2_path_t *path, int depth)
{
        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( depth < 0 )
                depth = path->depth - 1;

        if ( path->depth == 0 && depth < 0 )
                return IDMEFV2_VALUE_TYPE_CLASS;

        return path->elem[depth].value_type;
}



static inline int invalidate(idmefv2_path_t *path)
{
        int ret;

        gl_lock_lock(path->mutex);

        if ( path->refcount == 1 ) {
                gl_lock_unlock(path->mutex);
                return 0; /* not cached */
        }

        /*
         * We can modify the path only if the caller
         * is the only entity that has pointer to it.
         *
         * The path should have refcount equal to 1 or 2.
         * If refcount is equal to 1, it means that only the caller
         * has the pointer to the path, so we can modify it.
         *
         * If refcount is equal to 2, that means that the path
         * can also be present in the cached_path hash,
         * so it should be removed from the hash before modification.
         * If, however, refcount is 2 but the path is not present
         * in the hash, we cannot continue, as there exists
         * another entity that has pointer to the path.
         */

        if ( path->refcount > 2 ) {
                gl_lock_unlock(path->mutex);
                return -1;
        }

        if ( path->refcount == 2 ) {
                gl_lock_lock(cached_path_mutex);
                ret = libidmefv2_hash_elem_destroy(cached_path, path);
                gl_lock_unlock(cached_path_mutex);

                if ( ret == 0 )
                        path->refcount--;  /* path was present in a hash */
                else {
                        gl_lock_unlock(path->mutex);
                        return -1; /* path was not present in a hash and refcount != 1 */
                }
        }

        gl_lock_unlock(path->mutex);

        return 0; /* successfully invalidated */
}



/**
 * idmefv2_path_set_index:
 * @path: Pointer to an #idmefv2_path_t object.
 * @depth: Depth of @path to set @index for.
 * @index: Index for the provided element @depth.
 *
 * Modifies @index for the element located at @depth of provided @path.
 * This function is only applicable for element that accept listed value.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_path_set_index(idmefv2_path_t *path, unsigned int depth, int index)
{
        int ret;

        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(depth < path->depth, libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_DEPTH));

        if ( index == INDEX_FORBIDDEN || index == INDEX_KEY )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_RESERVED);

        if ( path->elem[depth].index == INDEX_FORBIDDEN )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_FORBIDDEN);

        ret = invalidate(path);
        if ( ret < 0 )
                return ret;

        if ( path->elem[depth].index == INDEX_KEY )
                free(path->elem[depth].index_key);

        path->elem[depth].index = index;

        ret = build_name(path);
        if ( ret < 0 )
                return ret;

        return 0;
}



/**
 * idmefv2_path_undefine_index:
 * @path: Pointer to an #idmefv2_path_t object.
 * @depth: Depth of @path to undefine the index for.
 *
 * Modifies the element located at @depth of provided @path so that it's
 * index is undefined.
 *
 * This function is only applicable for element that accept listed value.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_path_undefine_index(idmefv2_path_t *path, unsigned int depth)
{
        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        return idmefv2_path_set_index(path, depth, INDEX_UNDEFINED);
}



/**
 * idmefv2_path_get_index:
 * @path: Pointer to an #idmefv2_path_t object.
 * @depth: Depth of @path to retrieve the index from.
 *
 * Gets the current index for element located at @depth of @path.
 * This function is only applicable for element that accepts listed value.
 *
 * Returns: The element index, or a negative value if an error occured.
 */
int idmefv2_path_get_index(const idmefv2_path_t *path, unsigned int depth)
{
        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(depth < path->depth, libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_DEPTH));

        if ( path->elem[depth].index == INDEX_UNDEFINED )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_UNDEFINED);

        if ( path->elem[depth].index == INDEX_FORBIDDEN )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_FORBIDDEN);

        return path->elem[depth].index;
}



int idmefv2_path_get_key(const idmefv2_path_t *path, unsigned int depth, const char **key)
{
        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(depth < path->depth, libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_DEPTH));

        if ( path->elem[depth].index == INDEX_UNDEFINED )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_UNDEFINED);

        if ( path->elem[depth].index == INDEX_FORBIDDEN )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_FORBIDDEN);

        if ( path->elem[depth].index != INDEX_KEY )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_INDEX_FORBIDDEN);

        *key = path->elem[depth].index_key;
        return 0;
}



/**
 * idmefv2_path_make_child:
 * @path: Pointer to an #idmefv2_path_t object.
 * @child_name: Name of the child element to create.
 * @index: Index for @child_name, if applicable.
 *
 * Modifies @path so that it points to the child node identified by @child_name,
 * children of the current path. That is if the path is currently pointing to
 * alert.classification, and @child_name is set to "text", @path will be
 * modified to point to alert.classification.text.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_path_make_child(idmefv2_path_t *path, const char *child_name, int index)
{
        int ret;
        char buf[16] = { 0 };
        idmefv2_class_id_t class;
        idmefv2_class_child_id_t child;

        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(child_name, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( path->depth > MAX_DEPTH - 1 )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_DEPTH);

        class = idmefv2_path_get_class(path, -1);

        child = idmefv2_class_find_child(class, child_name);
        if ( child < 0 )
                return child;

        ret = invalidate(path);
        if ( ret < 0 )
                return ret;

        if ( index >= 0 && idmefv2_class_is_child_list(class, child) )
             snprintf(buf, sizeof(buf), "(%d)", index);

        snprintf(path->name + strlen(path->name), sizeof(path->name) - strlen(path->name), "%s%s%s",
                 (path->depth > 0) ? "." : "", child_name, buf);

        path->depth++;

        path->elem[path->depth - 1].position = child;
        if ( idmefv2_class_is_child_list(class, child) )
                path->elem[path->depth - 1].index = (index < 0) ? INDEX_UNDEFINED : index;
        else
                path->elem[path->depth - 1].index = INDEX_FORBIDDEN;

        path->elem[path->depth - 1].class = idmefv2_class_get_child_class(class, child);
        path->elem[path->depth - 1].value_type = idmefv2_class_get_child_value_type(class, child);

        return 0;
}


/**
 * idmefv2_path_make_parent:
 * @path: Pointer to an #idmefv2_path_t object.
 *
 * Removes the last element of the path. That is, if @path is currently pointing to
 * alert.classification, @path will be modified to point to alert.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_path_make_parent(idmefv2_path_t *path)
{
        int ret;
        char *ptr;

        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( path->depth == 0 )
                return libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_PARENT_ROOT);

        ret = invalidate(path);
        if ( ret < 0 )
                return ret;

        path->depth--;

        if ( path->name[0] ) {
                ptr = strrchr(path->name, '.');
                if ( ! ptr )
                        ptr = path->name; /* top-level path */

                *ptr = '\0';
        }

        return 0;
}



/**
 * idmefv2_path_destroy:
 * @path: Pointer to an #idmefv2_path_t object.
 *
 * Destroys the provided @path object.
 */
void idmefv2_path_destroy(idmefv2_path_t *path)
{
        libidmefv2_return_if_fail(path);

        gl_lock_lock(path->mutex);

        if ( --path->refcount ) {
                gl_lock_unlock(path->mutex);
                return;
        }

        gl_lock_unlock(path->mutex);
        gl_lock_destroy(path->mutex);
        free(path);
}



/**
 * idmefv2_path_ncompare:
 * @p1: Pointer to an #idmefv2_path_t object.
 * @p2: Pointer to another #idmefv2_path_t object.
 * @depth: Maximum depth to use for path comparison.
 *
 * Compares @p1 and @p2 elements up to @depth.
 *
 * Returns: 0 if both element match, a negative value otherwise.
 */
int idmefv2_path_ncompare(const idmefv2_path_t *p1, const idmefv2_path_t *p2, unsigned int depth)
{
        unsigned int i;

        libidmefv2_return_val_if_fail(p1, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(p2, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        for ( i = 0; i < depth; i++ ) {

                if ( p1->elem[i].index != p2->elem[i].index )
                        return -1;

                if ( p1->elem[i].index == INDEX_KEY && strcmp(p1->elem[i].index_key, p2->elem[i].index_key) != 0 )
                        return -1;

                if ( p1->elem[i].position - p2->elem[i].position )
                        return -1;
        }

        return 0;
}



/**
 * idmefv2_path_compare:
 * @p1: Pointer to an #idmefv2_path_t object.
 * @p2: Pointer to another #idmefv2_path_t object.
 *
 * Compares @p1 and @p2 elements.
 *
 * Returns: 0 if both element match, a negative value otherwise.
 */
int idmefv2_path_compare(const idmefv2_path_t *p1, const idmefv2_path_t *p2)
{
        libidmefv2_return_val_if_fail(p1, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(p2, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( p1->depth != p2->depth )
                return -1;

        return idmefv2_path_ncompare(p1, p2, p1->depth);
}




/**
 * idmefv2_path_clone:
 * @src: Pointer to an #idmefv2_path_t object.
 * @dst: Address where to store the copy of @src.
 *
 * Clones @src and stores the result in the provided @dst address.
 *
 * Returns: 0 on success, a negative value otherwise.
 */
int idmefv2_path_clone(const idmefv2_path_t *src, idmefv2_path_t **dst)
{
        unsigned int i;

        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        *dst = calloc(1, sizeof(**dst));
        if ( ! *dst )
                return libidmefv2_error_from_errno(errno);

        memcpy(*dst, src, sizeof(**dst));

        for ( i = 0; i < src->depth; i++ ) {
                if ( ! src->elem[i].index_key )
                        continue;

                (*dst)->elem[i].index_key = strdup(src->elem[i].index_key);
                if ( ! (*dst)->elem[i].index_key ) {
                        while ( --i ) {
                                if ( (*dst)->elem[i].index_key )
                                        free((*dst)->elem[i].index_key);
                        }

                        free(*dst);
                        return libidmefv2_error_from_errno(errno);
                }
        }

        (*dst)->refcount = 1;
        gl_lock_init((*dst)->mutex);

        return 0;
}



/**
 * idmefv2_path_check_operator:
 * @path: Pointer to a #idmefv2_path_t object.
 * @op: Operator to check compatibility with.
 *
 * Check whether @op can apply to value pointed to by @path.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_path_check_operator(const idmefv2_path_t *path, idmefv2_criterion_operator_t op)
{
        libidmefv2_return_val_if_fail(path, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return idmefv2_value_type_check_operator(idmefv2_path_get_value_type(path, -1), op);
}



/**
 * idmefv2_path_get_applicable_operators:
 * @path: Pointer to a #idmefv2_path_t object.
 * @result: Pointer to storage for applicable operator.
 *
 * Retrieve all applicable operator that might be used by the type of
 * value pointed to by @path.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_path_get_applicable_operators(const idmefv2_path_t *path, idmefv2_criterion_operator_t *result)
{
        libidmefv2_return_val_if_fail(path && result, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return idmefv2_value_type_get_applicable_operators(idmefv2_path_get_value_type(path, -1), result);
}



/**
 * idmefv2_path_ref:
 * @path: Pointer to an #idmefv2_path_t object.
 *
 * Increases @path reference count.
 *
 * idmefv2_path_destroy() will destroy the refcount until it reaches 0,
 * at which point the path will be destroyed.
 *
 * Returns: The provided @path is returned.
 */
idmefv2_path_t *idmefv2_path_ref(idmefv2_path_t *path)
{
        libidmefv2_return_val_if_fail(path, NULL);

        gl_lock_lock(path->mutex);
        path->refcount++;
        gl_lock_unlock(path->mutex);

        return path;
}



/**
 * idmefv2_path_is_ambiguous:
 * @path: Pointer to an #idmefv2_path_t object.
 *
 * Returns TRUE if @path contain elements that are supposed
 * to be listed, but for which no index were provided.
 *
 * Returns: TRUE if the object is ambiguous, FALSE otherwise.
 */
libidmefv2_bool_t idmefv2_path_is_ambiguous(const idmefv2_path_t *path)
{
        size_t i;

        libidmefv2_return_val_if_fail(path, FALSE);

        for ( i = 0; i < path->depth; i++ ) {
                if ( path->elem[i].index == INDEX_UNDEFINED )
                        return TRUE;
        }

        return FALSE;
}



/**
 * idmefv2_path_has_lists:
 * @path: Pointer to an #idmefv2_path_t object.
 *
 * Returns: the number of listed element within @path.
 */
int idmefv2_path_has_lists(const idmefv2_path_t *path)
{
        size_t i;
        int ret = 0;

        libidmefv2_return_val_if_fail(path, 0);

        for ( i = 0; i < path->depth; i++ ) {
                if ( path->elem[i].index != INDEX_FORBIDDEN )
                        ret++;
        }

        return ret;
}



libidmefv2_bool_t idmefv2_path_is_list(const idmefv2_path_t *path, int depth)
{
        libidmefv2_return_val_if_fail(path, FALSE);

        if ( depth < 0 )
                depth = path->depth - 1;

        return path->elem[depth].index != INDEX_FORBIDDEN;
}




/**
 * idmefv2_path_get_depth:
 * @path: Pointer to an #idmefv2_path_t object.
 *
 * Returns: @depth number of elements.
 */
unsigned int idmefv2_path_get_depth(const idmefv2_path_t *path)
{
        libidmefv2_return_val_if_fail(path, 0);
        return path->depth;
}



/**
 * idmefv2_path_get_name:
 * @path: Pointer to an #idmefv2_path_t object.
 * @depth: Depth of the @path element to get the name from.
 *
 * Returns the full path name if the provided @depth is -1, or the specific
 * element name if depth is set. That is, for a @path pointing to
 * "alert.classification.text": A depth of -1 would return "alert.classification.text";
 * a depth of 0 would return "alert"; a depth of 1 would return "classification"; and
 * a depth of 2 would return "text".
 *
 * Returns: @path name relative to the provided @dept.
 */
const char *idmefv2_path_get_name(const idmefv2_path_t *path, int depth)
{
        const char *ret;
        const idmefv2_path_element_t *elem;

        libidmefv2_return_val_if_fail(path, NULL);
        libidmefv2_return_val_if_fail(depth < 0 || (size_t) depth < path->depth, NULL);

        if ( depth < 0 )
                return path->name;

        elem = &path->elem[depth];

        if ( depth > 0 && (elem->class < 0 || elem->value_type == IDMEFV2_VALUE_TYPE_ENUM) )
                ret = idmefv2_class_get_child_name(path->elem[depth - 1].class, elem->position);
        else
                ret = idmefv2_class_get_name(elem->class);

        return ret;
}



void _idmefv2_path_cache_lock(void)
{
        gl_lock_lock(cached_path_mutex);

        if ( cached_path )
                libidmefv2_hash_iterate(cached_path, path_lock_cb);
}



void _idmefv2_path_cache_reinit(void)
{
        gl_lock_init(cached_path_mutex);

        if ( cached_path )
                libidmefv2_hash_iterate(cached_path, path_reinit_cb);
}




void _idmefv2_path_cache_unlock(void)
{
        if ( cached_path )
                libidmefv2_hash_iterate(cached_path, path_unlock_cb);

        gl_lock_unlock(cached_path_mutex);
}



void _idmefv2_path_cache_destroy(void)
{
        if ( ! cached_path )
                return;

        flush_cache = TRUE;
        libidmefv2_hash_destroy(cached_path);
        flush_cache = FALSE;
}

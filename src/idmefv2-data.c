/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
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

#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "libidmefv2-error.h"
#include "libidmefv2-inttypes.h"
#include "libidmefv2-string.h"
#include "common.h"
#include "idmefv2-data.h"


/*
 * Data structure may be free'd
 */
#define IDMEFV2_DATA_OWN_STRUCTURE  0x1

/*
 * Data content may be free'd
 */
#define IDMEFV2_DATA_OWN_DATA       0x2


#define IDMEFV2_DATA_DECL(idmefv2_data_type, c_type, name, fname)         \
int idmefv2_data_new_ ## name(idmefv2_data_t **nd, c_type val)            \
{                                                                     \
        int ret;                                                      \
                                                                      \
        ret = idmefv2_data_new(nd);                                     \
        if ( ret < 0 )                                                \
                return ret;                                           \
                                                                      \
        idmefv2_data_set_ ## fname(*nd, val);                           \
                                                                      \
        return ret;                                                   \
}                                                                     \
                                                                      \
void idmefv2_data_set_ ## name(idmefv2_data_t *ptr, c_type val)           \
{                                                                     \
        libidmefv2_return_if_fail(ptr);                                  \
        idmefv2_data_destroy_internal(ptr);                             \
        ptr->type = idmefv2_data_type;                                  \
        ptr->len = sizeof(val);                                       \
        ptr->data.fname ## _data = val;                               \
}                                                                     \
                                                                      \
c_type idmefv2_data_get_ ## name(const idmefv2_data_t *ptr)               \
{                                                                     \
        return ptr->data.fname ## _data;                              \
}


IDMEFV2_DATA_DECL(IDMEFV2_DATA_TYPE_CHAR, char, char, char)
IDMEFV2_DATA_DECL(IDMEFV2_DATA_TYPE_BYTE, uint8_t, byte, byte)
IDMEFV2_DATA_DECL(IDMEFV2_DATA_TYPE_INT, uint32_t, uint32, int)
IDMEFV2_DATA_DECL(IDMEFV2_DATA_TYPE_INT, uint64_t, uint64, int)
IDMEFV2_DATA_DECL(IDMEFV2_DATA_TYPE_INT, int64_t, int, int)
IDMEFV2_DATA_DECL(IDMEFV2_DATA_TYPE_FLOAT, float, float, float)


int idmefv2_data_new(idmefv2_data_t **data)
{
        *data = calloc(1, sizeof(**data));
        if ( ! *data )
                return libidmefv2_error_from_errno(errno);

        (*data)->refcount = 1;
        (*data)->flags |= IDMEFV2_DATA_OWN_STRUCTURE;

        return 0;
}


int idmefv2_data_new_time(idmefv2_data_t **data, idmefv2_time_t *time)
{
        int ret;

        ret = idmefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        (*data)->len = 1;
        (*data)->type = IDMEFV2_DATA_TYPE_TIME;
        (*data)->flags |= IDMEFV2_DATA_OWN_DATA;
        (*data)->data.ro_data = time;

        return 0;
}


void idmefv2_data_set_time(idmefv2_data_t *data, idmefv2_time_t *time)
{
        data->len = 1;
        data->type = IDMEFV2_DATA_TYPE_TIME;
        data->flags |= IDMEFV2_DATA_OWN_DATA;
        data->data.ro_data = time;
}

idmefv2_data_t *idmefv2_data_ref(idmefv2_data_t *data)
{
        libidmefv2_return_val_if_fail(data, NULL);

        data->refcount++;
        return data;
}



int idmefv2_data_set_ptr_ref_fast(idmefv2_data_t *data, idmefv2_data_type_t type, const void *ptr, size_t len)
{
        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        idmefv2_data_destroy_internal(data);

        data->type = type;
        data->data.ro_data = ptr;
        data->len = len;

        return 0;
}



int idmefv2_data_set_ptr_dup_fast(idmefv2_data_t *data, idmefv2_data_type_t type, const void *ptr, size_t len)
{
        void *new;

        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        idmefv2_data_destroy_internal(data);

        new = malloc(len);
        if ( ! new )
                return -1;

        memcpy(new, ptr, len);

        data->type = type;
        data->data.rw_data = new;
        data->len = len;
        data->flags |= IDMEFV2_DATA_OWN_DATA;

        return 0;
}



int idmefv2_data_set_ptr_nodup_fast(idmefv2_data_t *data, idmefv2_data_type_t type, void *ptr, size_t len)
{
        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        idmefv2_data_destroy_internal(data);

        data->type = type;
        data->data.rw_data= ptr;
        data->len = len;
        data->flags |= IDMEFV2_DATA_OWN_DATA;

        return 0;
}



int idmefv2_data_new_ptr_ref_fast(idmefv2_data_t **data, idmefv2_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = idmefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_data_set_ptr_ref_fast(*data, type, ptr, len);
        if ( ret < 0 )
                idmefv2_data_destroy(*data);

        return ret;
}



int idmefv2_data_new_ptr_dup_fast(idmefv2_data_t **data, idmefv2_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = idmefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_data_set_ptr_dup_fast(*data, type, ptr, len);
        if ( ret < 0 )
                idmefv2_data_destroy(*data);

        return ret;
}



int idmefv2_data_new_ptr_nodup_fast(idmefv2_data_t **data, idmefv2_data_type_t type, void *ptr, size_t len)
{
        int ret;

        ret = idmefv2_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_data_set_ptr_nodup_fast(*data, type, ptr, len);
        if ( ret < 0 )
                idmefv2_data_destroy(*data);

        return ret;
}




/**
 * idmefv2_data_copy_ref:
 * @src: Source #idmefv2_data_t object.
 * @dst: Destination #idmefv2_data_t object.
 *
 * Makes @dst reference the same buffer as @src.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_data_copy_ref(const idmefv2_data_t *src, idmefv2_data_t *dst)
{
        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(dst, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        idmefv2_data_destroy_internal(dst);

        dst->type = src->type;
        dst->len = src->len;
        dst->data = src->data;
        dst->flags &= ~IDMEFV2_DATA_OWN_DATA;

        return 0;
}




/**
 * idmefv2_data_copy_dup:
 * @src: Source #idmefv2_data_t object.
 * @dst: Destination #idmefv2_data_t object.
 *
 * Copies @src to @dst, including the associated buffer.
 * This is an alternative to idmefv2_data_clone().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_data_copy_dup(const idmefv2_data_t *src, idmefv2_data_t *dst)
{
        int ret;

        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(dst, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        idmefv2_data_destroy_internal(dst);

        dst->type = src->type;
        dst->flags |= IDMEFV2_DATA_OWN_DATA;
        dst->len = src->len;

        if ( src->type == IDMEFV2_DATA_TYPE_TIME ) {
                ret = idmefv2_time_clone((idmefv2_time_t *) src->data.rw_data, (idmefv2_time_t **) &dst->data.rw_data);
                if ( ret < 0 )
                        return ret;
        }

        else if ( src->type == IDMEFV2_DATA_TYPE_CHAR_STRING || src->type == IDMEFV2_DATA_TYPE_BYTE_STRING ) {
                dst->data.rw_data = malloc(src->len);
                if ( ! dst->data.rw_data )
                        return -1;

                memcpy(dst->data.rw_data, src->data.ro_data, src->len);
        } else {
                dst->data = src->data;
        }

        return 0;
}



int idmefv2_data_clone(const idmefv2_data_t *src, idmefv2_data_t **dst)
{
        int ret;

        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = idmefv2_data_new(dst);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_data_copy_dup(src, *dst);
        if ( ret < 0 )
                idmefv2_data_destroy(*dst);

        return ret;
}



const char *idmefv2_data_get_char_string(const idmefv2_data_t *data)
{
        libidmefv2_return_val_if_fail(data, NULL);
        return data->data.ro_data;
}



const unsigned char *idmefv2_data_get_byte_string(const idmefv2_data_t *data)
{
        libidmefv2_return_val_if_fail(data, NULL);
        return data->data.ro_data;
}



/**
 * idmefv2_data_get_type
 * @data: Pointer to an #idmefv2_data_t object.
 *
 * Returns: the type of the embedded data.
 */
idmefv2_data_type_t idmefv2_data_get_type(const idmefv2_data_t *data)
{
        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return data->type;
}




/**
 * idmefv2_data_get_len:
 * @data: Pointer to an #idmefv2_data_t object.
 *
 * Returns: the length of data contained within @data object.
 */
size_t idmefv2_data_get_len(const idmefv2_data_t *data)
{
        libidmefv2_return_val_if_fail(data, 0);
        return data->len;
}




/**
 * idmefv2_data_get_data:
 * @data: Pointer to an #idmefv2_data_t object.
 *
 * Returns: the data contained within @data object.
 */
const void *idmefv2_data_get_data(const idmefv2_data_t *data)
{
        libidmefv2_return_val_if_fail(data, NULL);

        switch ( data->type ) {
        case IDMEFV2_DATA_TYPE_UNKNOWN:
                return NULL;

        case IDMEFV2_DATA_TYPE_CHAR_STRING:
        case IDMEFV2_DATA_TYPE_BYTE_STRING:
        case IDMEFV2_DATA_TYPE_TIME:
                return data->data.ro_data;

        default:
                return &data->data;
        }

        return NULL;
}



/**
 * idmefv2_data_is_empty:
 * @data: Pointer to an #idmefv2_data_t object.
 *
 * Returns: TRUE if empty, FALSE otherwise.
 */
libidmefv2_bool_t idmefv2_data_is_empty(const idmefv2_data_t *data)
{
        libidmefv2_return_val_if_fail(data, TRUE);
        return (data->len == 0) ? TRUE : FALSE;
}



static int bytes_to_string(libidmefv2_string_t *out, const unsigned char *src, size_t size)
{
        char c;
        int ret;
        static const char b64tbl[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        while ( size ) {
                ret = libidmefv2_string_ncat(out, &b64tbl[(src[0] >> 2) & 0x3f], 1);
                if ( ret < 0 )
                        return ret;

                c = b64tbl[((src[0] << 4) + ((--size) ? src[1] >> 4 : 0)) & 0x3f];

                ret = libidmefv2_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                c = (size) ? b64tbl[((src[1] << 2) + ((--size) ? src[2] >> 6 : 0)) & 0x3f] : '=';

                ret = libidmefv2_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                c = (size && size--) ? b64tbl[src[2] & 0x3f] : '=';

                ret = libidmefv2_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                src += 3;
        }

        return 0;
}




/**
 * idmefv2_data_to_string:
 * @data: Pointer to an #idmefv2_data_t object.
 * @out: Pointer to a #libidmefv2_string_t to store the formated data into.
 *
 * Formats data contained within @data to be printable,
 * and stores the result in the provided @out buffer.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int idmefv2_data_to_string(const idmefv2_data_t *data, libidmefv2_string_t *out)
{
        int ret = 0;

        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(out, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        switch ( data->type ) {
        case IDMEFV2_DATA_TYPE_UNKNOWN:
                return 0;

        case IDMEFV2_DATA_TYPE_CHAR:
                ret = libidmefv2_string_sprintf(out, "%c", data->data.char_data);
                break;

        case IDMEFV2_DATA_TYPE_BYTE:
                /*
                 * %hh convertion specifier is not portable.
                 */
                ret = libidmefv2_string_sprintf(out, "%u", (unsigned int) data->data.byte_data);
                break;

        case IDMEFV2_DATA_TYPE_UINT32:
        case IDMEFV2_DATA_TYPE_INT:
                ret = libidmefv2_string_sprintf(out, "%" LIBIDMEFV2_PRId64, data->data.int_data);
                break;

        case IDMEFV2_DATA_TYPE_FLOAT:
                ret = libidmefv2_string_sprintf(out, "%f", data->data.float_data);
                break;

        case IDMEFV2_DATA_TYPE_CHAR_STRING:
                ret = libidmefv2_string_sprintf(out, "%s", (const char *) data->data.ro_data);
                break;

        case IDMEFV2_DATA_TYPE_BYTE_STRING:
                ret = bytes_to_string(out, data->data.ro_data, data->len);
                break;

        case IDMEFV2_DATA_TYPE_TIME:
                ret = idmefv2_time_to_string(data->data.ro_data, out);
                break;
        }

        return ret;
}



/*
 *  This function cannot be declared static because it is invoked
 *  from idmefv2-tree-wrap.c
 */
void idmefv2_data_destroy_internal(idmefv2_data_t *ptr)
{
        libidmefv2_return_if_fail(ptr);

        if ( ptr->type == IDMEFV2_DATA_TYPE_TIME && ptr->flags & IDMEFV2_DATA_OWN_DATA )
                idmefv2_time_destroy(ptr->data.rw_data);

        else if ( (ptr->type == IDMEFV2_DATA_TYPE_CHAR_STRING || ptr->type == IDMEFV2_DATA_TYPE_BYTE_STRING) &&
             ptr->flags & IDMEFV2_DATA_OWN_DATA ) {
                free(ptr->data.rw_data);
                ptr->data.rw_data = NULL;
        }

        /*
         * free() should be done by the caller
         */
}




/**
 * idmefv2_data_destroy:
 * @data: Pointer to an #idmefv2_data_t object.
 *
 * Frees @data. The buffer pointed by @data will be freed if
 * the @data object is marked as _dup or _nodup.
 */
void idmefv2_data_destroy(idmefv2_data_t *data)
{
        libidmefv2_return_if_fail(data);

        if ( --data->refcount )
                return;

        idmefv2_data_destroy_internal(data);

        if ( data->flags & IDMEFV2_DATA_OWN_STRUCTURE )
                free(data);
}


int idmefv2_data_new_char_string_ref_fast(idmefv2_data_t **data, const char *ptr, size_t len)
{
        return idmefv2_data_new_ptr_ref_fast(data, IDMEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int idmefv2_data_new_char_string_dup_fast(idmefv2_data_t **data, const char *ptr, size_t len)
{
        return idmefv2_data_new_ptr_dup_fast(data, IDMEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int idmefv2_data_new_char_string_nodup_fast(idmefv2_data_t **data, char *ptr, size_t len)
{
        return idmefv2_data_new_ptr_nodup_fast(data, IDMEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int idmefv2_data_set_char_string_ref_fast(idmefv2_data_t *data, const char *ptr, size_t len)
{
        return idmefv2_data_set_ptr_ref_fast(data, IDMEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int idmefv2_data_set_char_string_dup_fast(idmefv2_data_t *data, const char *ptr, size_t len)
{
        return idmefv2_data_set_ptr_dup_fast(data, IDMEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int idmefv2_data_set_char_string_nodup_fast(idmefv2_data_t *data, char *ptr, size_t len)
{
        return idmefv2_data_set_ptr_nodup_fast(data, IDMEFV2_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}


int idmefv2_data_new_char_string_ref(idmefv2_data_t **data, const char *ptr)
{
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return idmefv2_data_new_char_string_ref_fast(data, ptr, strlen(ptr));
}

int idmefv2_data_new_char_string_dup(idmefv2_data_t **data, const char *ptr)
{
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return idmefv2_data_new_char_string_dup_fast(data, ptr, strlen(ptr));
}

int idmefv2_data_new_char_string_nodup(idmefv2_data_t **data, char *ptr)
{
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return idmefv2_data_new_char_string_nodup_fast(data, ptr, strlen(ptr));
}

int idmefv2_data_set_char_string_ref(idmefv2_data_t *data, const char *ptr)
{
        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        return idmefv2_data_set_char_string_ref_fast(data, ptr, strlen(ptr));
}

int idmefv2_data_set_char_string_dup(idmefv2_data_t *data, const char *ptr)
{
        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        return idmefv2_data_set_char_string_dup_fast(data, ptr, strlen(ptr));
}

int idmefv2_data_set_char_string_nodup(idmefv2_data_t *data, char *ptr)
{
        libidmefv2_return_val_if_fail(data, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(ptr, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        return idmefv2_data_set_char_string_nodup_fast(data, ptr, strlen(ptr));
}


/*
 *
 */
int idmefv2_data_new_byte_string_ref(idmefv2_data_t **data, const unsigned char *ptr, size_t len)
{
        return idmefv2_data_new_ptr_ref_fast(data, IDMEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int idmefv2_data_new_byte_string_dup(idmefv2_data_t **data, const unsigned char *ptr, size_t len)
{
        return idmefv2_data_new_ptr_dup_fast(data, IDMEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int idmefv2_data_new_byte_string_nodup(idmefv2_data_t **data, unsigned char *ptr, size_t len)
{
        return idmefv2_data_new_ptr_nodup_fast(data, IDMEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int idmefv2_data_set_byte_string_ref(idmefv2_data_t *data, const unsigned char *ptr, size_t len)
{
        return idmefv2_data_set_ptr_ref_fast(data, IDMEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int idmefv2_data_set_byte_string_dup(idmefv2_data_t *data, const unsigned char *ptr, size_t len)
{
        return idmefv2_data_set_ptr_dup_fast(data, IDMEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}


int idmefv2_data_set_byte_string_nodup(idmefv2_data_t *data, unsigned char *ptr, size_t len)
{
        return idmefv2_data_set_ptr_nodup_fast(data, IDMEFV2_DATA_TYPE_BYTE_STRING, ptr, len);
}



int idmefv2_data_compare(const idmefv2_data_t *data1, const idmefv2_data_t *data2)
{
        if ( ! data1 && ! data2 )
                return 0;

        else if ( ! data1 || ! data2 )
                return (data1) ? 1 : -1;

        else if ( data1->len != data2->len )
                return (data1->len > data2->len) ? 1 : -1;

        else if ( data1->type != data2->type )
                return -1;

        if ( data1->type == IDMEFV2_DATA_TYPE_TIME )
                return idmefv2_time_compare(data1->data.ro_data, data2->data.ro_data);

        else if ( data1->type == IDMEFV2_DATA_TYPE_CHAR_STRING || data1->type == IDMEFV2_DATA_TYPE_BYTE_STRING )
                return memcmp(data1->data.ro_data, data2->data.ro_data, data1->len);

        else
                return memcmp(&data1->data.char_data, &data2->data.char_data, data1->len);
}

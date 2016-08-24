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

/*
 * This code include an heavily modified version of the libidmefv2-strbuf
 * API made by Krzysztof Zaraska, that is now part of libidmefv2-string.
 */

#include "config.h"
#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

#include "common.h"
#include "libidmefv2-log.h"
#include "libidmefv2-inttypes.h"
#include "libidmefv2-string.h"


#define LIBIDMEFV2_ERROR_SOURCE_DEFAULT LIBIDMEFV2_ERROR_SOURCE_STRING
#include "libidmefv2-error.h"


#define BUFFER_CHUNK_SIZE 16
#define BUFFER_GROWTH_FACTOR 1.5


/*
 * String structure may be free'd
 */
#define LIBIDMEFV2_STRING_OWN_STRUCTURE  0x1

/*
 * String data may be free'd
 */
#define LIBIDMEFV2_STRING_OWN_DATA       0x2




#if ! defined (LIBIDMEFV2_VA_COPY)

# if defined (__GNUC__) && defined (__PPC__) && (defined (_CALL_SYSV) || defined (_WIN32))
#  define LIBIDMEFV2_VA_COPY(ap1, ap2)     (*(ap1) = *(ap2))

# elif defined (LIBIDMEFV2_VA_COPY_AS_ARRAY)
#  define LIBIDMEFV2_VA_COPY(ap1, ap2)     memmove ((ap1), (ap2), sizeof(va_list))

# else /* va_list is a pointer */
#  define LIBIDMEFV2_VA_COPY(ap1, ap2)     ((ap1) = (ap2))

# endif
#endif


#define STRING_RETURN_IF_INVALID(str, len) do {                                                                \
        libidmefv2_return_val_if_fail((len + 1) > len,                                                            \
                                    libidmefv2_error_verbose(LIBIDMEFV2_ERROR_INVAL_LENGTH,                          \
                                                          "string length warning: wrap around would occur"));  \
                                                                                                               \
        libidmefv2_return_val_if_fail(str[len] == 0,                                                              \
                                   libidmefv2_error_verbose(LIBIDMEFV2_ERROR_STRING_NOT_NULL_TERMINATED,             \
                                                         "string warning: not nul terminated"));               \
} while(0)



struct libidmefv2_string {
        LIBIDMEFV2_LINKED_OBJECT;

        int flags;
        int refcount;

        union {
                char *rwbuf;
                const char *robuf;
        } data;

        size_t size;
        size_t index;
};


static int string_buf_alloc(libidmefv2_string_t *string, size_t len)
{
        /*
         * include room for terminating \0.
         */
        string->data.rwbuf = malloc(len + 1);
        if ( ! string->data.rwbuf )
                return libidmefv2_error_from_errno(errno);

        string->index = len;
        string->size = len + 1;

        return 0;
}



static void string_buf_copy(libidmefv2_string_t *string, const char *buf, size_t len)
{
        assert(len < string->size);

        memcpy(string->data.rwbuf, buf, len);
        string->data.rwbuf[len] = '\0';
}



static int make_string_own(libidmefv2_string_t *s, size_t len)
{
        char *ptr;

        ptr = malloc(len);
        if ( ! ptr )
                return libidmefv2_error_from_errno(errno);

        if ( s->data.robuf )
                memcpy(ptr, s->data.robuf, s->index + 1);

        s->size = len;
        s->data.rwbuf = ptr;
        s->flags |= LIBIDMEFV2_STRING_OWN_DATA;

        return 0;
}



static int allocate_more_chunk_if_needed(libidmefv2_string_t *s, size_t needed_len)
{
        int ret;
        char *ptr;
        size_t len;

        if ( ! needed_len )
                len = BUFFER_CHUNK_SIZE;
        else
                len = MAX(needed_len - (s->size - s->index), s->size * BUFFER_GROWTH_FACTOR);

        if ( s->size + len < s->size )
                return libidmefv2_error(LIBIDMEFV2_ERROR_INVAL_LENGTH);

        if ( s->flags & LIBIDMEFV2_STRING_OWN_DATA ) {
                ptr = _libidmefv2_realloc(s->data.rwbuf, s->size + len);
                if ( ! ptr )
                        return libidmefv2_error_from_errno(errno);

                s->size += len;
                s->data.rwbuf = ptr;
        }

        else {
                ret = make_string_own(s, s->size + len);
                if ( ret < 0 )
                        return ret;
        }

        return 0;
}



/**
 * libidmefv2_string_new:
 * @string: Pointer where to store the created #libidmefv2_string_t.
 *
 * Create a new #libidmefv2_string_t object, and store in in @string.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_new(libidmefv2_string_t **string)
{
        *string = calloc(1, sizeof(**string));
        if ( ! *string )
                return libidmefv2_error_from_errno(errno);

        (*string)->refcount = 1;
        libidmefv2_list_init(&(*string)->_list);
        (*string)->flags = LIBIDMEFV2_STRING_OWN_STRUCTURE;

        return 0;
}




/**
 * libidmefv2_string_ref:
 * @string: Pointer to a #libidmefv2_string_t object to reference.
 *
 * Increase @string reference count.
 *
 * Returns: @string.
 */
libidmefv2_string_t *libidmefv2_string_ref(libidmefv2_string_t *string)
{
        libidmefv2_return_val_if_fail(string, NULL);

        string->refcount++;
        return string;
}



/**
 * libidmefv2_string_new_dup_fast:
 * @string: Pointer where to store the created #libidmefv2_string_t object.
 * @str: Initial string value.
 * @len: Lenght of @str.
 *
 * Create a new #libidmefv2_string_t object with a copy of @str as it's
 * initial value.  The copy is owned by the @string and will be freed
 * upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int libidmefv2_string_new_dup_fast(libidmefv2_string_t **string, const char *str, size_t len)
{
        int ret;

        libidmefv2_return_val_if_fail(str, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        STRING_RETURN_IF_INVALID(str, len);

        ret = libidmefv2_string_new(string);
        if ( ret < 0 )
                return ret;

        ret = string_buf_alloc(*string, len);
        if ( ret < 0 )
                return ret;

        string_buf_copy(*string, str, len);
        (*string)->flags |= LIBIDMEFV2_STRING_OWN_DATA;

        return 0;
}



/**
 * libidmefv2_string_new_dup:
 * @string: Pointer where to store the created #libidmefv2_string_t object.
 * @str: Initial string value.
 *
 * Create a new #libidmefv2_string_t object with a copy of @str as it's
 * initial value. The copy is owned by the @string and will be freed
 * upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int libidmefv2_string_new_dup(libidmefv2_string_t **string, const char *str)
{
        libidmefv2_return_val_if_fail(str, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return libidmefv2_string_new_dup_fast(string, str, strlen(str));
}



/**
 * libidmefv2_string_new_nodup_fast:
 * @string: Pointer where to store the created #libidmefv2_string_t object.
 * @str: Initial string value.
 * @len: Lenght of @str.
 *
 * Create a new #libidmefv2_string_t object with a reference to @str as
 * initial value.  @str is owned by @string and will be freed upon
 * libidmefv2_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int libidmefv2_string_new_nodup_fast(libidmefv2_string_t **string, char *str, size_t len)
{
        int ret;

        libidmefv2_return_val_if_fail(str, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        STRING_RETURN_IF_INVALID(str, len);

        ret = libidmefv2_string_new(string);
        if ( ret < 0 )
                return ret;

        (*string)->index = len;
        (*string)->size = len + 1;
        (*string)->data.rwbuf = str;
        (*string)->flags |= LIBIDMEFV2_STRING_OWN_DATA;

        return 0;
}



/**
 * libidmefv2_string_new_nodup:
 * @string: Pointer where to store the created #libidmefv2_string_t object.
 * @str: Initial string value.
 *
 * Create a new #libidmefv2_string_t object with a reference to @str as
 * initial value.  @str is owned by @string and will be freed upon
 * libidmefv2_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int libidmefv2_string_new_nodup(libidmefv2_string_t **string, char *str)
{
        libidmefv2_return_val_if_fail(str, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return libidmefv2_string_new_nodup_fast(string, str, strlen(str));
}



/**
 * libidmefv2_string_new_ref_fast:
 * @string: Pointer where to store the created #libidmefv2_string_t object.
 * @str: Initial string value.
 * @len: Length of @str.
 *
 * Create a new #libidmefv2_string_t object with a reference to @str as
 * initial value. @str won't be freed upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int libidmefv2_string_new_ref_fast(libidmefv2_string_t **string, const char *buf, size_t len)
{
        int ret;

        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        STRING_RETURN_IF_INVALID(buf, len);

        ret = libidmefv2_string_new(string);
        if ( ret < 0 )
                return ret;

        (*string)->index = len;
        (*string)->size = len + 1;
        (*string)->data.robuf = buf;

        return 0;
}



/**
 * libidmefv2_string_new_ref:
 * @string: Pointer where to store the created #libidmefv2_string_t object.
 * @str: Initial string value.
 *
 * Create a new #libidmefv2_string_t object with a reference to @str as
 * initial value. @str won't be freed upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int libidmefv2_string_new_ref(libidmefv2_string_t **string, const char *str)
{
        libidmefv2_return_val_if_fail(str, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return libidmefv2_string_new_ref_fast(string, str, strlen(str));
}



/**
 * libidmefv2_string_set_dup_fast:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @buf: String to store in @string.
 * @len: Lenght of @buf.
 *
 * Store a copy of the string pointed by @buf in @string.
 * The @buf copy will be freed upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_set_dup_fast(libidmefv2_string_t *string, const char *buf, size_t len)
{
        int ret;

        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        STRING_RETURN_IF_INVALID(buf, len);

        libidmefv2_string_destroy_internal(string);

        ret = string_buf_alloc(string, len);
        if ( ret < 0 )
                return ret;

        string_buf_copy(string, buf, len);
        string->flags |= LIBIDMEFV2_STRING_OWN_DATA;

        return 0;
}



/**
 * libidmefv2_string_set_dup:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @buf: String to store in @string.
 *
 * Store a copy of the string pointed by @buf in @string.
 * The @buf copy will be freed upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_set_dup(libidmefv2_string_t *string, const char *buf)
{
        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        return libidmefv2_string_set_dup_fast(string, buf, strlen(buf));
}



/**
 * libidmefv2_string_set_nodup_fast:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @buf: String to store in @string.
 * @len: Lenght of @buf.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf will be freed upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_set_nodup_fast(libidmefv2_string_t *string, char *buf, size_t len)
{
        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        STRING_RETURN_IF_INVALID(buf, len);

        libidmefv2_string_destroy_internal(string);

        string->index = len;
        string->size = len + 1;
        string->data.rwbuf = buf;

        string->flags |= LIBIDMEFV2_STRING_OWN_DATA;

        return 0;
}




/**
 * libidmefv2_string_set_nodup:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @buf: String to store in @string.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf will be freed upon libidmefv2_string_destroy().
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_set_nodup(libidmefv2_string_t *string, char *buf)
{
        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        return libidmefv2_string_set_nodup_fast(string, buf, strlen(buf));
}



/**
 * libidmefv2_string_set_ref_fast:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @buf: String to store in @string.
 * @len: Lenght of @buf.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf value won't be modified or freed.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_set_ref_fast(libidmefv2_string_t *string, const char *buf, size_t len)
{
        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        STRING_RETURN_IF_INVALID(buf, len);

        libidmefv2_string_destroy_internal(string);

        string->index = len;
        string->size = len + 1;
        string->data.robuf = buf;

        string->flags &= ~LIBIDMEFV2_STRING_OWN_DATA;

        return 0;
}



/**
 * libidmefv2_string_set_ref:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @buf: String to store in @string.
 *
 * Store a reference to the string pointed by @buf in @string.
 * The referenced @buf value won't be modified or freed.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_set_ref(libidmefv2_string_t *string, const char *buf)
{
        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(buf, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        return libidmefv2_string_set_ref_fast(string, buf, strlen(buf));
}




/**
 * libidmefv2_string_copy_ref:
 * @src: Pointer to a #libidmefv2_string_t object to copy data from.
 * @dst: Pointer to a #libidmefv2_string_t object to copy data to.
 *
 * Reference @src content within @dst.
 * The referenced content won't be modified or freed.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_copy_ref(const libidmefv2_string_t *src, libidmefv2_string_t *dst)
{
        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(dst, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        libidmefv2_string_destroy_internal(dst);

        dst->size = src->size;
        dst->index = src->index;
        dst->data.robuf = src->data.robuf;
        dst->flags &= ~LIBIDMEFV2_STRING_OWN_DATA;

        return 0;
}



/**
 * libidmefv2_string_copy_dup:
 * @src: Pointer to a #libidmefv2_string_t object to copy data from.
 * @dst: Pointer to a #libidmefv2_string_t object to copy data to.
 *
 * Copy @src content within @dst.
 * The content is owned by @src and independent of @dst.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_copy_dup(const libidmefv2_string_t *src, libidmefv2_string_t *dst)
{
        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(dst, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        libidmefv2_string_destroy_internal(dst);

        dst->size = src->size;
        dst->index = src->index;
        dst->flags |= LIBIDMEFV2_STRING_OWN_DATA;

        if ( src->size ) {
                dst->data.rwbuf = malloc(src->size);
                if ( ! dst->data.rwbuf )
                        return libidmefv2_error_from_errno(errno);

                string_buf_copy(dst, src->data.robuf, src->index);
        }

        return 0;
}



/**
 * libidmefv2_string_clone:
 * @src: Pointer to an existing #libidmefv2_string_t object.
 * @dst: Pointer to an address where to store the created #libidmefv2_string_t object.
 *
 * Clone @src within a new #libidmefv2_string_t object stored into @dst.
 * Data carried by @dst and @src are independant.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_clone(const libidmefv2_string_t *src, libidmefv2_string_t **dst)
{
        int ret;

        libidmefv2_return_val_if_fail(src, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        ret = libidmefv2_string_new(dst);
        if ( ret < 0 )
                return ret;

        (*dst)->size = src->size;
        (*dst)->index = src->index;
        (*dst)->flags |= LIBIDMEFV2_STRING_OWN_DATA;

        if ( src->size ) {
                (*dst)->data.rwbuf = malloc(src->size);
                if ( ! (*dst)->data.rwbuf ) {
                        libidmefv2_string_destroy(*dst);
                        return libidmefv2_error_from_errno(errno);
                }

                string_buf_copy(*dst, src->data.robuf, src->index);
        }

        return 0;
}



/**
 * libidmefv2_string_get_len:
 * @string: Pointer to a #libidmefv2_string_t object.
 *
 * Return the length of the string carried by @string object.
 *
 * Returns: The length of the string owned by @string.
 */
size_t libidmefv2_string_get_len(const libidmefv2_string_t *string)
{
        libidmefv2_return_val_if_fail(string, 0);
        return string->index;
}



/**
 * libidmefv2_string_get_string_or_default:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @def: Default value to a return in case @string is empty.
 *
 * Return the string carried on by @string object, or @def if it is empty.
 * There should be no operation done on the returned string since it is still
 * owned by @string.
 *
 * Returns: The string owned by @string or @def.
 */
const char *libidmefv2_string_get_string_or_default(const libidmefv2_string_t *string, const char *def)
{
        libidmefv2_return_val_if_fail(string, NULL);
        return string->data.robuf ? string->data.robuf : def;
}



/**
 * libidmefv2_string_get_string:
 * @string: Pointer to a #libidmefv2_string_t object.
 *
 * Return the string carried on by @string object.
 * There should be no operation done on the returned string since
 * it is still owned by @string.
 *
 * Returns: The string owned by @string if any.
 */
const char *libidmefv2_string_get_string(const libidmefv2_string_t *string)
{
        libidmefv2_return_val_if_fail(string, NULL);
        return string->data.robuf;
}




/**
 * libidmefv2_string_get_string_released:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @outptr: Pointer to an address where to store the released string.
 *
 * Get @string content, and release it so that further operation on
 * @string won't modify the returned content.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int libidmefv2_string_get_string_released(libidmefv2_string_t *string, char **outptr)
{
        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        *outptr = NULL;

        if ( ! string->index )
                return 0;

        if ( ! (string->flags & LIBIDMEFV2_STRING_OWN_DATA) ) {
                *outptr = strdup(string->data.robuf);
                return (*outptr) ? 0 : libidmefv2_error_from_errno(errno);
        }

        if ( string->index + 1 <= string->index )
                return libidmefv2_error(LIBIDMEFV2_ERROR_INVAL_LENGTH);

        *outptr = _libidmefv2_realloc(string->data.rwbuf, string->index + 1);
        if ( ! *outptr )
                return libidmefv2_error_from_errno(errno);

        string->size = 0;
        string->index = 0;
        string->data.rwbuf = NULL;

        return 0;
}




/**
 * libidmefv2_string_is_empty:
 * @string: Pointer to a #libidmefv2_string_t object.
 *
 * Check whether @string is empty.
 *
 * Returns: TRUE if @string is empty, FALSE otherwise.
 */
libidmefv2_bool_t libidmefv2_string_is_empty(const libidmefv2_string_t *string)
{
        libidmefv2_return_val_if_fail(string, TRUE);
        return (string->index == 0) ? TRUE : FALSE;
}



/*
 *  This function cannot be declared static because its invoked
 *  from idmefv2-tree-wrap.c
 */
void libidmefv2_string_destroy_internal(libidmefv2_string_t *string)
{
        libidmefv2_return_if_fail(string);

        if ( (string->flags & LIBIDMEFV2_STRING_OWN_DATA) && string->data.rwbuf )
                free(string->data.rwbuf);

        string->data.rwbuf = NULL;
        string->index = string->size = 0;

        /*
         * free() should be done by the caller
         */
}



/**
 * libidmefv2_string_destroy:
 * @string: Pointer to a #libidmefv2_string_t object.
 *
 * Decrease refcount and destroy @string.
 * @string content content is destroyed if applicable (dup and nodup,
 * or a referenced string that have been modified.
 */
void libidmefv2_string_destroy(libidmefv2_string_t *string)
{
        libidmefv2_return_if_fail(string);

        if ( --string->refcount )
                return;

        if ( ! libidmefv2_list_is_empty(&string->_list) )
                libidmefv2_list_del_init(&string->_list);

        libidmefv2_string_destroy_internal(string);

        if ( string->flags & LIBIDMEFV2_STRING_OWN_STRUCTURE )
                free(string);
}




/**
 * libidmefv2_string_vprintf:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @fmt: Format string to use.
 * @ap: Variable argument list.
 *
 * Produce output according to @fmt, storing argument provided in @ap
 * variable argument list, and write the output to the given @string.
 * See sprintf(3) for more information on @fmt format.
 *
 * Returns: The number of characters written, or a negative value if an error occured.
 */
int libidmefv2_string_vprintf(libidmefv2_string_t *string, const char *fmt, va_list ap)
{
        int ret;
        va_list bkp;

        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(fmt, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( ! (string->flags & LIBIDMEFV2_STRING_OWN_DATA) ) {

                ret = allocate_more_chunk_if_needed(string, 0);
                if ( ret < 0 )
                        return ret;
        }

        LIBIDMEFV2_VA_COPY(bkp, ap);
        ret = vsnprintf(string->data.rwbuf + string->index, string->size - string->index, fmt, ap);

        /*
         * From sprintf(3) on GNU/Linux:
         *
         * snprintf  and vsnprintf do not write more than
         * size bytes (including the trailing '\0'), and return -1 if
         * the  output  was truncated due to this limit.  (Thus until
         * glibc 2.0.6. Since glibc 2.1 these  functions  follow  the
         * C99  standard and return the number of characters (exclud-
         * ing the trailing '\0') which would have  been  written  to
         * the final string if enough space had been available.)
         */
        if ( ret >= 0 && (size_t) ret < string->size - string->index ) {
                string->index += ret;
                goto end;
        }

        ret = allocate_more_chunk_if_needed(string, (ret < 0) ? 0 : ret + 1);
        if ( ret < 0 )
                goto end;

        ret = libidmefv2_string_vprintf(string, fmt, bkp);

 end:
        va_end(bkp);
        return ret;
}




/**
 * libidmefv2_string_sprintf:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @fmt: Format string to use.
 * @...: Variable argument list.
 *
 * Produce output according to @fmt, and write output to the given
 * @string. See snprintf(3) to learn more about @fmt format.
 *
 * Returns: The number of characters written, or a negative value if an error occured.
 */
int libidmefv2_string_sprintf(libidmefv2_string_t *string, const char *fmt, ...)
{
        int ret;
        va_list ap;

        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(fmt, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        va_start(ap, fmt);
        ret = libidmefv2_string_vprintf(string, fmt, ap);
        va_end(ap);

        return ret;
}



/**
 * libidmefv2_string_cat:
 * @dst: Pointer to a #libidmefv2_string_t object.
 * @str: Pointer to a string.
 * @len: Length of @str to copy.
 *
 * The libidmefv2_string_ncat() function appends @len characters from @str to
 * the @dst #libidmefv2_string_t object over-writing the `\0' character at the
 * end of @dst, and then adds a termi-nating `\0' character.
 *
 * Returns: @len, or a negative value if an error occured.
 */
int libidmefv2_string_ncat(libidmefv2_string_t *dst, const char *str, size_t len)
{
        int ret;

        libidmefv2_return_val_if_fail(dst, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(str, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( dst->flags & LIBIDMEFV2_STRING_OWN_DATA && len < (dst->size - dst->index) ) {

                memcpy(dst->data.rwbuf + dst->index, str, len);

                dst->index += len;
                dst->data.rwbuf[dst->index] = '\0';

                return len;
        }

        if ( len + 1 < len )
                return libidmefv2_error(LIBIDMEFV2_ERROR_INVAL_LENGTH);

        ret = allocate_more_chunk_if_needed(dst, len + 1);
        if ( ret < 0 )
                return ret;

        return libidmefv2_string_ncat(dst, str, len);
}



/**
 * libidmefv2_string_cat:
 * @dst: Pointer to a #libidmefv2_string_t object.
 * @str: Pointer to a string.
 *
 * The libidmefv2_string_cat() function appends the @str string to the @dst
 * libidmefv2_string_t object over-writing the `\0' character at the end of
 * @dst, and then adds a termi-nating `\0' character.
 *
 * Returns: @len, or a negative value if an error occured.
 */
int libidmefv2_string_cat(libidmefv2_string_t *dst, const char *str)
{
        libidmefv2_return_val_if_fail(dst, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(str, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        return libidmefv2_string_ncat(dst, str, strlen(str));
}




/**
 * libidmefv2_string_truncate:
 * @string: Pointer to a #libidmefv2_string_t object.
 * @len: New @string size
 *
 * Truncate @string content.
 */
int libidmefv2_string_truncate(libidmefv2_string_t *string, size_t len)
{
        int ret;

        libidmefv2_return_val_if_fail(string, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));
        libidmefv2_return_val_if_fail(len <= string->index, libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION));

        if ( len && ! (string->flags & LIBIDMEFV2_STRING_OWN_DATA) ) {
                ret = make_string_own(string, 0);
                if ( ret < 0 )
                        return ret;
        }

        if ( string->data.rwbuf )
                *(string->data.rwbuf + len) = '\0';
        else
                string->data.robuf = NULL;

        string->index = len;
        return 0;
}



/**
 * libidmefv2_string_clear:
 * @string: Pointer to a #libidmefv2_string_t object.
 *
 * Reset @string content to zero.
 */
void libidmefv2_string_clear(libidmefv2_string_t *string)
{
        libidmefv2_string_truncate(string, 0);
}




/**
 * libidmefv2_string_compare:
 * @str1: Pointer to a #libidmefv2_string_t object to compare with @str2.
 * @str2: Pointer to a #libidmefv2_string_t object to compare with @str1.
 *
 * Compare @str1 and @str2.
 *
 * Returns: 0 if @str1 and @str2 value are equal, a negative value otherwise.
 */
int libidmefv2_string_compare(const libidmefv2_string_t *str1, const libidmefv2_string_t *str2)
{
        if ( ! str1 && ! str2 )
                return 0;

        else if ( ! str1 || ! str2 )
                return -1;

        else if ( str1->index != str2->index )
                return -1;

        if ( str1->index == 0 )
                return 0;

        return strcmp(str1->data.robuf, str2->data.robuf) == 0 ? 0 : -1;
}

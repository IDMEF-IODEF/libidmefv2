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

#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "libidmefv2-log.h"
#include "libidmefv2-string.h"
#include "libidmefv2-error.h"
#include "libidmefv2-inttypes.h"
#include "idmefv2.h"
#include "idmefv2-tree-wrap.h"
#include "idmefv2-additional-data.h"
#include "idmefv2-message-id.h"





#define IDMEFV2_ADDITIONAL_DATA_ACCESSOR(name, type)                                                              \
int idmefv2_additional_data_new_ ## name ## _ref_fast(idmefv2_additional_data_t **ad, const char *data, size_t len) \
{                                                                                                               \
        return idmefv2_additional_data_new_ptr_ref_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_new_ ## name ## _ref(idmefv2_additional_data_t **ad, const char *data)                  \
{                                                                                                               \
        return idmefv2_additional_data_new_ ## name ## _ref_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_set_ ## name ## _ref_fast(idmefv2_additional_data_t *ad, const char *data, size_t len)  \
{                                                                                                               \
        return idmefv2_additional_data_set_ptr_ref_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_set_ ## name ## _ref(idmefv2_additional_data_t *ad, const char *data)                   \
{                                                                                                               \
        return idmefv2_additional_data_set_ ## name ## _ref_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_new_ ## name ## _dup_fast(idmefv2_additional_data_t **ad, const char *data, size_t len) \
{                                                                                                               \
        return idmefv2_additional_data_new_ptr_dup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_new_ ## name ## _dup(idmefv2_additional_data_t **ad, const char *data)                  \
{                                                                                                               \
        return idmefv2_additional_data_new_ ## name ## _dup_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_set_ ## name ## _dup_fast(idmefv2_additional_data_t *ad, const char *data, size_t len)  \
{                                                                                                               \
        return idmefv2_additional_data_set_ptr_dup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_set_ ## name ## _dup(idmefv2_additional_data_t *ad, const char *data)                   \
{                                                                                                               \
        return idmefv2_additional_data_set_ ## name ## _dup_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_new_ ## name ## _nodup_fast(idmefv2_additional_data_t **ad, char *data, size_t len)     \
{                                                                                                               \
        return idmefv2_additional_data_new_ptr_nodup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);\
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_new_ ## name ## _nodup(idmefv2_additional_data_t **ad, char *data)                      \
{                                                                                                               \
        return idmefv2_additional_data_new_ ## name ## _nodup_fast(ad, data, strlen(data));                       \
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_set_ ## name ## _nodup_fast(idmefv2_additional_data_t *ad, char *data, size_t len)      \
{                                                                                                               \
        return idmefv2_additional_data_set_ptr_nodup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);\
}                                                                                                               \
                                                                                                                \
int idmefv2_additional_data_set_ ## name ## _nodup(idmefv2_additional_data_t *ad, char *data)                       \
{                                                                                                               \
        return idmefv2_additional_data_set_ ## name ## _nodup_fast(ad, data, strlen(data));                       \
}


#define IDMEFV2_ADDITIONAL_DATA_SIMPLE(d_type, d_name, ad_type, c_type, name)                \
int idmefv2_additional_data_new_ ## name(idmefv2_additional_data_t **ret, c_type val)        \
{                                                                                       \
        int retval;                                                                     \
                                                                                        \
        retval = idmefv2_additional_data_new(ret);                                               \
        if ( retval < 0 )                                                                      \
                return retval;                                                                \
                                                                                        \
        idmefv2_additional_data_set_type(*ret, ad_type);                                        \
        idmefv2_data_set_ ## d_name(idmefv2_additional_data_get_data(*ret), val);                \
                                                                                        \
        return retval;                                                                        \
}                                                                                        \
                                                                                        \
void idmefv2_additional_data_set_ ## name(idmefv2_additional_data_t *ptr, c_type val)        \
{                                                                                        \
        idmefv2_additional_data_set_type(ptr, ad_type);                                        \
        idmefv2_data_set_ ## d_name(idmefv2_additional_data_get_data(ptr), val);                \
}                                                                                        \
                                                                                        \
c_type idmefv2_additional_data_get_ ## name(idmefv2_additional_data_t *ptr)                        \
{                                                                                        \
        return idmefv2_data_get_ ## d_name(idmefv2_additional_data_get_data(ptr));                \
}



/*
 * Backward compatibility stuff, remove once 0.9.0 is released.
 */
int idmefv2_additional_data_new_ptr_ref_fast(idmefv2_additional_data_t **nd,
                                           idmefv2_additional_data_type_t type,
                                           const void *ptr, size_t len);

int idmefv2_additional_data_new_ptr_dup_fast(idmefv2_additional_data_t **nd,
                                           idmefv2_additional_data_type_t type,
                                           const void *ptr, size_t len);

int idmefv2_additional_data_new_ptr_nodup_fast(idmefv2_additional_data_t **nd,
                                             idmefv2_additional_data_type_t type,
                                             void *ptr, size_t len);

int idmefv2_additional_data_set_ptr_ref_fast(idmefv2_additional_data_t *data,
                                           idmefv2_additional_data_type_t type, const void *ptr, size_t len);

int idmefv2_additional_data_set_ptr_dup_fast(idmefv2_additional_data_t *data,
                                           idmefv2_additional_data_type_t type, const void *ptr, size_t len);

int idmefv2_additional_data_set_ptr_nodup_fast(idmefv2_additional_data_t *data,
                                             idmefv2_additional_data_type_t type, void *ptr, size_t len);



static const struct {
        idmefv2_additional_data_type_t ad_type;
        idmefv2_data_type_t d_type;
        size_t len;
} idmefv2_additional_data_type_table[] = {
        { IDMEFV2_ADDITIONAL_DATA_TYPE_STRING, IDMEFV2_DATA_TYPE_CHAR_STRING, 0 },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE, IDMEFV2_DATA_TYPE_BYTE, sizeof(uint8_t) },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_CHARACTER, IDMEFV2_DATA_TYPE_CHAR, sizeof(char) },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_DATE_TIME, IDMEFV2_DATA_TYPE_CHAR_STRING, 0 },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_INTEGER, IDMEFV2_DATA_TYPE_INT, sizeof(uint64_t) },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_NTPSTAMP, IDMEFV2_DATA_TYPE_INT, sizeof(uint64_t) },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_PORTLIST, IDMEFV2_DATA_TYPE_CHAR_STRING, 0 },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_REAL, IDMEFV2_DATA_TYPE_FLOAT, sizeof(float) },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_BOOLEAN, IDMEFV2_DATA_TYPE_BYTE, sizeof(libidmefv2_bool_t) },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING, IDMEFV2_DATA_TYPE_BYTE_STRING, 0 },
        { IDMEFV2_ADDITIONAL_DATA_TYPE_XML, IDMEFV2_DATA_TYPE_CHAR_STRING, 0 }
};



static int check_type(idmefv2_additional_data_type_t type, const unsigned char *buf, size_t len)
{
        if ( type < 0 || (size_t) type >= sizeof(idmefv2_additional_data_type_table) / sizeof(*idmefv2_additional_data_type_table) )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "Invalid IDMEFV2AdditionalData type specified");

        if ( idmefv2_additional_data_type_table[type].len != 0 &&
             len > idmefv2_additional_data_type_table[type].len )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "Provided value length does not match specified type length");

        if ( idmefv2_additional_data_type_table[type].len == 0 && len < 1 )
                return libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "Invalid value length for this type");

        if ( type == IDMEFV2_ADDITIONAL_DATA_TYPE_STRING ||
             type == IDMEFV2_ADDITIONAL_DATA_TYPE_DATE_TIME ||
             type == IDMEFV2_ADDITIONAL_DATA_TYPE_PORTLIST ||
             type == IDMEFV2_ADDITIONAL_DATA_TYPE_XML )
                return buf[len - 1] == '\0' ? 0 : libidmefv2_error_verbose(LIBIDMEFV2_ERROR_GENERIC, "String is not nul terminated");

        return 0;
}



static idmefv2_data_type_t idmefv2_additional_data_type_to_data_type(idmefv2_additional_data_type_t type)
{
        if ( type < 0 || (size_t) type >= sizeof(idmefv2_additional_data_type_table) / sizeof(*idmefv2_additional_data_type_table) )
                return IDMEFV2_DATA_TYPE_UNKNOWN;

        return idmefv2_additional_data_type_table[type].d_type;
}



int idmefv2_additional_data_new_ptr_ref_fast(idmefv2_additional_data_t **nd,
                                           idmefv2_additional_data_type_t type,
                                           const void *ptr, size_t len)
{
        int ret;
        idmefv2_data_type_t dtype;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(*nd, type);
        dtype = idmefv2_additional_data_type_to_data_type(type);

        ret = idmefv2_data_set_ptr_ref_fast(idmefv2_additional_data_get_data(*nd), dtype, ptr, len);
        if ( ret < 0 ) {
                idmefv2_additional_data_destroy(*nd);
                return ret;
        }

        return 0;
}



int idmefv2_additional_data_new_ptr_dup_fast(idmefv2_additional_data_t **nd,
                                           idmefv2_additional_data_type_t type,
                                           const void *ptr, size_t len)
{
        int ret;
        idmefv2_data_type_t dtype;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(*nd, type);
        dtype = idmefv2_additional_data_type_to_data_type(type);

        ret = idmefv2_data_set_ptr_dup_fast(idmefv2_additional_data_get_data(*nd), dtype, ptr, len);
        if ( ret < 0 ) {
                idmefv2_additional_data_destroy(*nd);
                return ret;
        }

        return 0;
}



int idmefv2_additional_data_new_ptr_nodup_fast(idmefv2_additional_data_t **nd,
                                             idmefv2_additional_data_type_t type,
                                             void *ptr, size_t len)
{
        int ret;
        idmefv2_data_type_t dtype;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(*nd, type);
        dtype = idmefv2_additional_data_type_to_data_type(type);

        ret = idmefv2_data_set_ptr_nodup_fast(idmefv2_additional_data_get_data(*nd), dtype, ptr, len);
        if ( ret < 0 ) {
                idmefv2_additional_data_destroy(*nd);
                return ret;
        }

        return ret;
}



int idmefv2_additional_data_set_ptr_ref_fast(idmefv2_additional_data_t *data,
                                           idmefv2_additional_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(data, type);

        return idmefv2_data_set_ptr_ref_fast(idmefv2_additional_data_get_data(data),
                                           idmefv2_additional_data_type_to_data_type(type), ptr, len);
}



int idmefv2_additional_data_set_ptr_dup_fast(idmefv2_additional_data_t *data,
                                           idmefv2_additional_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(data, type);

        return idmefv2_data_set_ptr_dup_fast(idmefv2_additional_data_get_data(data),
                                           idmefv2_additional_data_type_to_data_type(type), ptr, len);
}



int idmefv2_additional_data_set_ptr_nodup_fast(idmefv2_additional_data_t *data,
                                             idmefv2_additional_data_type_t type, void *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(data, type);

        return idmefv2_data_set_ptr_nodup_fast(idmefv2_additional_data_get_data(data),
                                             idmefv2_additional_data_type_to_data_type(type), ptr, len);
}




/*
 * Declare stuff
 */
IDMEFV2_ADDITIONAL_DATA_SIMPLE(IDMEFV2_DATA_TYPE_FLOAT, float, IDMEFV2_ADDITIONAL_DATA_TYPE_REAL, float, real)
IDMEFV2_ADDITIONAL_DATA_SIMPLE(IDMEFV2_DATA_TYPE_INT, int, IDMEFV2_ADDITIONAL_DATA_TYPE_INTEGER, uint32_t, integer)
IDMEFV2_ADDITIONAL_DATA_SIMPLE(IDMEFV2_DATA_TYPE_BYTE, byte, IDMEFV2_ADDITIONAL_DATA_TYPE_BOOLEAN, libidmefv2_bool_t, boolean)
IDMEFV2_ADDITIONAL_DATA_SIMPLE(IDMEFV2_DATA_TYPE_BYTE, byte, IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE, uint8_t, byte)
IDMEFV2_ADDITIONAL_DATA_SIMPLE(IDMEFV2_DATA_TYPE_CHAR, char, IDMEFV2_ADDITIONAL_DATA_TYPE_CHARACTER, char, character)

IDMEFV2_ADDITIONAL_DATA_ACCESSOR(xml, XML)
IDMEFV2_ADDITIONAL_DATA_ACCESSOR(string, STRING)
IDMEFV2_ADDITIONAL_DATA_ACCESSOR(ntpstamp, NTPSTAMP)
IDMEFV2_ADDITIONAL_DATA_ACCESSOR(portlist, PORTLIST)
IDMEFV2_ADDITIONAL_DATA_ACCESSOR(date_time, DATE_TIME)



/*
 * just make a pointer copy of the embedded data
 */
int idmefv2_additional_data_copy_ref(idmefv2_additional_data_t *src, idmefv2_additional_data_t *dst)
{
        int ret;

        ret = libidmefv2_string_copy_ref(idmefv2_additional_data_get_meaning(src), idmefv2_additional_data_get_meaning(dst));
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(dst, idmefv2_additional_data_get_type(src));

        return idmefv2_data_copy_ref(idmefv2_additional_data_get_data(src), idmefv2_additional_data_get_data(dst));
}



/*
 * also copy the content of the embedded data
 */
int idmefv2_additional_data_copy_dup(idmefv2_additional_data_t *src, idmefv2_additional_data_t *dst)
{
        int ret;

        ret = libidmefv2_string_copy_dup(idmefv2_additional_data_get_meaning(src), idmefv2_additional_data_get_meaning(dst));
        if ( ret < 0 )
                return ret;

        idmefv2_additional_data_set_type(dst, idmefv2_additional_data_get_type(src));

        return idmefv2_data_copy_dup(idmefv2_additional_data_get_data(src), idmefv2_additional_data_get_data(dst));
}



size_t idmefv2_additional_data_get_len(idmefv2_additional_data_t *data)
{
        return idmefv2_data_get_len(idmefv2_additional_data_get_data(data));
}



libidmefv2_bool_t idmefv2_additional_data_is_empty(idmefv2_additional_data_t *data)
{
        return idmefv2_data_is_empty(idmefv2_additional_data_get_data(data));
}



int idmefv2_additional_data_data_to_string(idmefv2_additional_data_t *ad, libidmefv2_string_t *out)
{
        int ret;
        uint64_t i;
        idmefv2_data_t *data;

        data = idmefv2_additional_data_get_data(ad);
        if ( idmefv2_data_is_empty(data) )
                return 0;

        switch ( idmefv2_additional_data_get_type(ad) ) {

        case IDMEFV2_ADDITIONAL_DATA_TYPE_NTPSTAMP:
                i = idmefv2_data_get_int(data);
                ret = libidmefv2_string_sprintf(out, "0x%" LIBIDMEFV2_PRIx32 ".0x%" LIBIDMEFV2_PRIx32, (uint32_t) (i >> 32), (uint32_t) i);
                break;

        default:
                ret = idmefv2_data_to_string(data, out);
                break;
        }

        return ret;
}


/*
 * byte-string specific stuff
 */
int idmefv2_additional_data_new_byte_string_ref(idmefv2_additional_data_t **ad, const unsigned char *data, size_t len)
{
        return idmefv2_additional_data_new_ptr_ref_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int idmefv2_additional_data_set_byte_string_ref(idmefv2_additional_data_t *ad, const unsigned char *data, size_t len)
{
         return idmefv2_additional_data_set_ptr_ref_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int idmefv2_additional_data_new_byte_string_dup(idmefv2_additional_data_t **ad, const unsigned char *data, size_t len)
{
        return idmefv2_additional_data_new_ptr_dup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int idmefv2_additional_data_set_byte_string_dup(idmefv2_additional_data_t *ad, const unsigned char *data, size_t len)
{
        return idmefv2_additional_data_set_ptr_dup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int idmefv2_additional_data_new_byte_string_nodup(idmefv2_additional_data_t **ad, unsigned char *data, size_t len)
{
        return idmefv2_additional_data_new_ptr_nodup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int idmefv2_additional_data_set_byte_string_nodup(idmefv2_additional_data_t *ad, unsigned char *data, size_t len)
{
        return idmefv2_additional_data_set_ptr_nodup_fast(ad, IDMEFV2_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}


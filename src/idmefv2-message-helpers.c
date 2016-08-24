/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
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

#include <stdlib.h>
#include <string.h>

#include "idmefv2.h"
#include "idmefv2-message-helpers.h"


/**
 * idmefv2_message_set_value:
 * @message: Pointer to an #idmefv2_message_t object.
 * @path: Path to be set within @message.
 * @value: Value to associate @message[@path] with.
 *
 * This function will set the @path member within @message to the
 * provided @value.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_message_set_value(idmefv2_message_t *message, const char *path, idmefv2_value_t *value)
{
        int ret;
        idmefv2_path_t *ip;

        ret = idmefv2_path_new_fast(&ip, path);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_path_set(ip, message, value);
        idmefv2_path_destroy(ip);

        return ret;
}



/**
 * idmefv2_message_get_value:
 * @message: Pointer to an #idmefv2_message_t object.
 * @path: Path to retrieve the value from within @message.
 * @value: Pointer where the result should be stored.
 *
 * Retrieve the value stored within @path of @message and store it
 * in the user provided @value.
 *
 * Returns: A positive value in case @path was successfully retrieved
 * 0 if the path is empty, or a negative value if an error occured.
 */
int idmefv2_message_get_value(idmefv2_message_t *message, const char *path, idmefv2_value_t **value)
{
        int ret;
        idmefv2_path_t *ip;

        ret = idmefv2_path_new_fast(&ip, path);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_path_get(ip, message, value);
        idmefv2_path_destroy(ip);

        return ret;
}



/**
 * idmefv2_message_set_string:
 * @message: Pointer to an #idmefv2_message_t object.
 * @path: Path to be set within @message.
 * @value: Value to associate @message[@path] with.
 *
 * This function will set the @path member within @message to the
 * provided @value, which will be converted to the corresponding
 * @path value type.
 *
 * Example:
 * idmefv2_message_set_string(message, "alert.classification.text", "MyText");
 * idmefv2_message_set_string(message, "alert.source(0).service.port", "1024");
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_message_set_string(idmefv2_message_t *message, const char *path, const char *value)
{
        int ret;
        idmefv2_value_t *iv;
        libidmefv2_string_t *str;

        ret = libidmefv2_string_new_dup(&str, value);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_value_new_string(&iv, str);
        if ( ret < 0 ) {
                libidmefv2_string_destroy(str);
                return ret;
        }

        ret = idmefv2_message_set_value(message, path, iv);
        idmefv2_value_destroy(iv);

        return ret;
}



/**
 * idmefv2_message_get_string:
 * @message: Pointer to an #idmefv2_message_t object.
 * @path: Path to retrieve the string from within @message.
 * @result: Pointer where the result should be stored.
 *
 * Retrieve the string stored within @path of @message and store it
 * in the user provided @result.
 *
 * The caller is responssible for freeing @result.
 *
 * Returns: A positive value in case @path was successfully retrieved
 * 0 if the path is empty, or a negative value if an error occured.
 */
int idmefv2_message_get_string(idmefv2_message_t *message, const char *path, char **result)
{
        int ret;
        idmefv2_value_t *iv;
        libidmefv2_string_t *str;

        ret = idmefv2_message_get_value(message, path, &iv);
        if ( ret <= 0 )
                return ret;

        if ( idmefv2_value_get_type(iv) != IDMEFV2_VALUE_TYPE_STRING ) {
                ret = _idmefv2_value_cast(iv, IDMEFV2_VALUE_TYPE_STRING, 0);
                if ( ret < 0 )
                        goto err;
        }

        if ( ! (str = idmefv2_value_get_string(iv)) ) {
                ret = -1;
                goto err;
        }

        if ( libidmefv2_string_is_empty(str) ) {
                *result = NULL;
                return 0;
        }

        *result = strdup(libidmefv2_string_get_string(str));
        ret = libidmefv2_string_get_len(str);

err:
        idmefv2_value_destroy(iv);
        return ret;
}



/**
 * idmefv2_message_set_number:
 * @message: Pointer to an #idmefv2_message_t object.
 * @path: Path to be set within @message.
 * @number: Value to associate @message[@path] with.
 *
 * This function will set the @path member within @message to the
 * provided @value, which will be converted to the @path value type.
 *
 * Example:
 * idmefv2_message_set_number(message, "alert.assessment.confidence.confidence", 0.123456);
 * idmefv2_message_set_number(message, "alert.source(0).service.port", 1024);
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_message_set_number(idmefv2_message_t *message, const char *path, double number)
{
        int ret;
        idmefv2_value_t *iv;

        ret = idmefv2_value_new_double(&iv, number);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_message_set_value(message, path, iv);
        idmefv2_value_destroy(iv);

        return ret;

}


/**
 * idmefv2_message_get_number:
 * @message: Pointer to an #idmefv2_message_t object.
 * @path: Path to retrieve the number from within @message.
 * @result: Pointer where the result should be stored.
 *
 * Retrieve the number stored within @path of @message and store it
 * in the user provided @result.
 *
 * Returns: A positive value in case @path was successfully retrieved
 * 0 if the path is empty, or a negative value if an error occured.
 */
int idmefv2_message_get_number(idmefv2_message_t *message, const char *path, double *result)
{
        int ret;
        idmefv2_value_t *iv;

        ret = idmefv2_message_get_value(message, path, &iv);
        if ( ret <= 0 )
                return ret;

        if ( idmefv2_value_get_type(iv) != IDMEFV2_VALUE_TYPE_DOUBLE ) {
                ret = _idmefv2_value_cast(iv, IDMEFV2_VALUE_TYPE_DOUBLE, 0);
                if ( ret < 0 )
                        goto err;
        }

        ret = 1;
        *result = idmefv2_value_get_double(iv);

err:
        idmefv2_value_destroy(iv);
        return ret;
}


/**
 * idmefv2_message_set_data:
 * @message: Pointer to an #idmefv2_message_t object.
 * @path: Path to be set within @message.
 * @data: Pointer to data to associate @message[@path] with.
 * @size: Size of the data pointed to by @data.
 *
 * This function will set the @path member within @message to the
 * provided @data of size @size.
 *
 * Returns: 0 on success, or a negative value if an error occured.
 */
int idmefv2_message_set_data(idmefv2_message_t *message, const char *path, const unsigned char *data, size_t size)
{
        int ret;
        idmefv2_data_t *id;
        idmefv2_value_t *iv;

        ret = idmefv2_data_new_byte_string_dup(&id, data, size);
        if ( ret < 0 )
                return ret;

        ret = idmefv2_value_new_data(&iv, id);
        if ( ret < 0 ) {
                idmefv2_data_destroy(id);
                return ret;
        }

        ret = idmefv2_message_set_value(message, path, iv);
        idmefv2_value_destroy(iv);

        return ret;
}


int idmefv2_message_get_data(idmefv2_message_t *message, const char *path, unsigned char **data, size_t *size)
{
        int ret;
        idmefv2_data_t *dp;
        idmefv2_value_t *iv;

        ret = idmefv2_message_get_value(message, path, &iv);
        if ( ret <= 0 )
                return ret;

        if ( idmefv2_value_get_type(iv) != IDMEFV2_VALUE_TYPE_DATA || ! (dp = idmefv2_value_get_data(iv)) ) {
                ret = -1;
                goto err;
        }

        *size = idmefv2_data_get_len(dp);
        *data = malloc(*size);
        if ( ! *data ) {
                ret = -1;
                goto err;
        }

        memcpy(*data, idmefv2_data_get_data(dp), *size);

err:
        idmefv2_value_destroy(iv);
        return ret;
}

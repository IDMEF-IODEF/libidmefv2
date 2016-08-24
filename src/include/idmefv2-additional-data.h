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

#ifndef _LIBIDMEFV2_IDMEFV2_ADDITIONAL_DATA_H
#define _LIBIDMEFV2_IDMEFV2_ADDITIONAL_DATA_H

#ifdef __cplusplus
 extern "C" {
#endif
         
/*
 * basic type
 */
int idmefv2_additional_data_new_real(idmefv2_additional_data_t **ret, float data);
int idmefv2_additional_data_new_byte(idmefv2_additional_data_t **ret, uint8_t byte);
int idmefv2_additional_data_new_integer(idmefv2_additional_data_t **ret, uint32_t data);
int idmefv2_additional_data_new_boolean(idmefv2_additional_data_t **ret, libidmefv2_bool_t data);
int idmefv2_additional_data_new_character(idmefv2_additional_data_t **ret, char data);

void idmefv2_additional_data_set_real(idmefv2_additional_data_t *ptr, float data);
void idmefv2_additional_data_set_byte(idmefv2_additional_data_t *ptr, uint8_t byte);
void idmefv2_additional_data_set_integer(idmefv2_additional_data_t *ptr, uint32_t data);
void idmefv2_additional_data_set_boolean(idmefv2_additional_data_t *ptr, libidmefv2_bool_t data);
void idmefv2_additional_data_set_character(idmefv2_additional_data_t *ptr, char data);


#define _IDMEFV2_ADDITIONAL_DATA_DECL(name) \
int idmefv2_additional_data_new_ ## name ## _ref_fast(idmefv2_additional_data_t **ad, const char *data, size_t len); \
int idmefv2_additional_data_new_ ## name ## _ref(idmefv2_additional_data_t **ad, const char *data); \
int idmefv2_additional_data_set_ ## name ## _ref_fast(idmefv2_additional_data_t *ad, const char *data, size_t len); \
int idmefv2_additional_data_set_ ## name ## _ref(idmefv2_additional_data_t *ad, const char *data); \
int idmefv2_additional_data_new_ ## name ## _dup_fast(idmefv2_additional_data_t **ad, const char *data, size_t len); \
int idmefv2_additional_data_new_ ## name ## _dup(idmefv2_additional_data_t **ad, const char *data); \
int idmefv2_additional_data_set_ ## name ## _dup_fast(idmefv2_additional_data_t *ad, const char *data, size_t len); \
int idmefv2_additional_data_set_ ## name ## _dup(idmefv2_additional_data_t *ad, const char *data); \
int idmefv2_additional_data_new_ ## name ## _nodup_fast(idmefv2_additional_data_t **ad, char *data, size_t len); \
int idmefv2_additional_data_new_ ## name ## _nodup(idmefv2_additional_data_t **ad, char *data); \
int idmefv2_additional_data_set_ ## name ## _nodup_fast(idmefv2_additional_data_t *ad, char *data, size_t len); \
int idmefv2_additional_data_set_ ## name ## _nodup(idmefv2_additional_data_t *ad, char *data);

_IDMEFV2_ADDITIONAL_DATA_DECL(string)
_IDMEFV2_ADDITIONAL_DATA_DECL(ntpstamp)
_IDMEFV2_ADDITIONAL_DATA_DECL(date_time)
_IDMEFV2_ADDITIONAL_DATA_DECL(portlist)
_IDMEFV2_ADDITIONAL_DATA_DECL(xml)

int idmefv2_additional_data_new_byte_string_ref(idmefv2_additional_data_t **ad, const unsigned char *data, size_t len);
int idmefv2_additional_data_set_byte_string_ref(idmefv2_additional_data_t *ad, const unsigned char *data, size_t len);
int idmefv2_additional_data_new_byte_string_dup(idmefv2_additional_data_t **ad, const unsigned char *data, size_t len);
int idmefv2_additional_data_set_byte_string_dup(idmefv2_additional_data_t *ad, const unsigned char *data, size_t len);
int idmefv2_additional_data_new_byte_string_nodup(idmefv2_additional_data_t **ad, unsigned char *data, size_t len);
int idmefv2_additional_data_set_byte_string_nodup(idmefv2_additional_data_t *ad, unsigned char *data, size_t len);
         

         
/*
 * copy / clone / destroy
 */
int idmefv2_additional_data_copy_ref(idmefv2_additional_data_t *src, idmefv2_additional_data_t *dst);
int idmefv2_additional_data_copy_dup(idmefv2_additional_data_t *src, idmefv2_additional_data_t *dst);

/*
 * Accessors
 */

float idmefv2_additional_data_get_real(idmefv2_additional_data_t *data);
uint32_t idmefv2_additional_data_get_integer(idmefv2_additional_data_t *data);
libidmefv2_bool_t idmefv2_additional_data_get_boolean(idmefv2_additional_data_t *data);
char idmefv2_additional_data_get_character(idmefv2_additional_data_t *data);
uint8_t idmefv2_additional_data_get_byte(idmefv2_additional_data_t *data);

size_t idmefv2_additional_data_get_len(idmefv2_additional_data_t *data);

libidmefv2_bool_t idmefv2_additional_data_is_empty(idmefv2_additional_data_t *data);

int idmefv2_additional_data_data_to_string(idmefv2_additional_data_t *ad, libidmefv2_string_t *out);

#ifdef __cplusplus
 }
#endif
         
#endif /* _LIBIDMEFV2_IDMEFV2_DATA_H */

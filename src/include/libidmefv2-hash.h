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

#ifndef _LIBIDMEFV2_HASH_H
#define _LIBIDMEFV2_HASH_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libidmefv2_hash libidmefv2_hash_t;

int libidmefv2_hash_new(libidmefv2_hash_t **hash,
                     unsigned int (*hash_func)(const void *),
                     int (*key_cmp_func)(const void *, const void *),
                     void (*key_destroy_func)(void *),
                     void (*value_destroy_func)(void *));

int libidmefv2_hash_new2(libidmefv2_hash_t **hash, size_t size,
                      unsigned int (*hash_func)(const void *),
                      int (*key_cmp_func)(const void *, const void *),
                      void (*key_destroy_func)(void *),
                     void (*value_destroy_func)(void *));

void libidmefv2_hash_destroy(libidmefv2_hash_t *hash);

int libidmefv2_hash_set(libidmefv2_hash_t *hash, void *key, void *value);

int libidmefv2_hash_add(libidmefv2_hash_t *hash, void *key, void *value);

void *libidmefv2_hash_get(libidmefv2_hash_t *hash, const void *key);

int libidmefv2_hash_elem_destroy(libidmefv2_hash_t *hash, const void *key);

void libidmefv2_hash_iterate(libidmefv2_hash_t *hash, void (*cb)(void *data));

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_HASH_H */

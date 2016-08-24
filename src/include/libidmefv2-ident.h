/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIDMEFV2_LIBIDMEFV2_IDENT_H
#define _LIBIDMEFV2_LIBIDMEFV2_IDENT_H

#include "libidmefv2-inttypes.h"
#include "libidmefv2-string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libidmefv2_ident libidmefv2_ident_t;

int libidmefv2_ident_generate(libidmefv2_ident_t *ident, libidmefv2_string_t *out);

uint64_t libidmefv2_ident_inc(libidmefv2_ident_t *ident);

void libidmefv2_ident_destroy(libidmefv2_ident_t *ident);

int libidmefv2_ident_new(libidmefv2_ident_t **ret);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_LIBIDMEFV2_IDENT_H */

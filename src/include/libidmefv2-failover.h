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

#ifndef _LIBIDMEFV2_LIBIDMEFV2_FAILOVER_H
#define _LIBIDMEFV2_LIBIDMEFV2_FAILOVER_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libidmefv2_failover libidmefv2_failover_t;

void libidmefv2_failover_destroy(libidmefv2_failover_t *failover);

int libidmefv2_failover_new(libidmefv2_failover_t **ret, const char *dirname);

void libidmefv2_failover_set_quota(libidmefv2_failover_t *failover, size_t limit);

int libidmefv2_failover_save_msg(libidmefv2_failover_t *failover, libidmefv2_msg_t *msg);

ssize_t libidmefv2_failover_get_saved_msg(libidmefv2_failover_t *failover, libidmefv2_msg_t **out);

unsigned long libidmefv2_failover_get_deleted_msg_count(libidmefv2_failover_t *failover);

unsigned long libidmefv2_failover_get_available_msg_count(libidmefv2_failover_t *failover);

void libidmefv2_failover_enable_transaction(libidmefv2_failover_t *failover);

void libidmefv2_failover_disable_transaction(libidmefv2_failover_t *failover);

int libidmefv2_failover_commit(libidmefv2_failover_t *failover, libidmefv2_msg_t *msg);

int libidmefv2_failover_rollback(libidmefv2_failover_t *failover, libidmefv2_msg_t *msg);

#ifdef __cplusplus
 }
#endif

#endif

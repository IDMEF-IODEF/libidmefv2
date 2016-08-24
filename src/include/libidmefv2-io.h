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

#ifndef _LIBIDMEFV2_LIBIDMEFV2_IO_H
#define _LIBIDMEFV2_LIBIDMEFV2_IO_H

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include "libidmefv2-inttypes.h"


typedef struct libidmefv2_io libidmefv2_io_t;

/*
 * Object creation / destruction functions.
 */
int libidmefv2_io_new(libidmefv2_io_t **ret);

void libidmefv2_io_destroy(libidmefv2_io_t *pio);

void libidmefv2_io_set_file_io(libidmefv2_io_t *pio, FILE *fd);

void libidmefv2_io_set_tls_io(libidmefv2_io_t *pio, void *tls);

void libidmefv2_io_set_sys_io(libidmefv2_io_t *pio, int fd);

int libidmefv2_io_set_buffer_io(libidmefv2_io_t *pio);


/*
 *
 */
void libidmefv2_io_set_fdptr(libidmefv2_io_t *pio, void *ptr);
void libidmefv2_io_set_write_callback(libidmefv2_io_t *pio, ssize_t (*write)(libidmefv2_io_t *io, const void *buf, size_t count));
void libidmefv2_io_set_read_callback(libidmefv2_io_t *pio, ssize_t (*read)(libidmefv2_io_t *io, void *buf, size_t count));
void libidmefv2_io_set_pending_callback(libidmefv2_io_t *pio, ssize_t (*pending)(libidmefv2_io_t *io));


/*
 * IO operations.
 */
int libidmefv2_io_close(libidmefv2_io_t *pio);

ssize_t libidmefv2_io_read(libidmefv2_io_t *pio, void *buf, size_t count);

ssize_t libidmefv2_io_read_wait(libidmefv2_io_t *pio, void *buf, size_t count);

ssize_t libidmefv2_io_read_delimited(libidmefv2_io_t *pio, unsigned char **buf);


ssize_t libidmefv2_io_write(libidmefv2_io_t *pio, const void *buf, size_t count);

ssize_t libidmefv2_io_write_delimited(libidmefv2_io_t *pio, const void *buf, uint16_t count);


ssize_t libidmefv2_io_forward(libidmefv2_io_t *dst, libidmefv2_io_t *src, size_t count);

int libidmefv2_io_get_fd(libidmefv2_io_t *pio);

void *libidmefv2_io_get_fdptr(libidmefv2_io_t *pio);

ssize_t libidmefv2_io_pending(libidmefv2_io_t *pio);

libidmefv2_bool_t libidmefv2_io_is_error_fatal(libidmefv2_io_t *pio, int error);

#ifdef __cplusplus
  }
#endif

#endif /* _LIBIDMEFV2_LIBIDMEFV2_IO_H */

/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
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

#ifndef _LIBIDMEFV2_IDMEFV2_OBJECT_PRV_H
#define _LIBIDMEFV2_IDMEFV2_OBJECT_PRV_H

#define IDMEFV2_OBJECT unsigned int _idmefv2_object_id
#define IDMEFV2_LINKED_OBJECT IDMEFV2_OBJECT; libidmefv2_list_t _list

struct idmefv2_object {
        IDMEFV2_OBJECT;
};

struct idmefv2_linked_object {
        IDMEFV2_OBJECT;
        libidmefv2_list_t _list;
};

#define idmefv2_linked_object_get_object(object) \
        (void *) libidmefv2_list_entry(object, struct idmefv2_linked_object, _list)

#endif /* _LIBIDMEFV2_IDMEFV2_OBJECT_PRV_H */

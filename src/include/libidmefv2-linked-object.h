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

#ifndef _LIBIDMEFV2_LIBIDMEFV2_LINKED_OBJECT_H
#define _LIBIDMEFV2_LIBIDMEFV2_LINKED_OBJECT_H


#include "libidmefv2-list.h"


#define LIBIDMEFV2_LINKED_OBJECT    \
        unsigned int _object_id; \
        libidmefv2_list_t _list


typedef struct {
        LIBIDMEFV2_LINKED_OBJECT;
} libidmefv2_linked_object_t;



static inline void libidmefv2_linked_object_del(libidmefv2_linked_object_t *obj) 
{
        libidmefv2_list_del(&obj->_list);
}



static inline void libidmefv2_linked_object_del_init(libidmefv2_linked_object_t *obj) 
{
        libidmefv2_list_del(&obj->_list);
        libidmefv2_list_init(&obj->_list);
}



static inline void libidmefv2_linked_object_add(libidmefv2_list_t *head, libidmefv2_linked_object_t *obj) 
{
        libidmefv2_list_add(head, &obj->_list);
}



static inline void libidmefv2_linked_object_add_tail(libidmefv2_list_t *head, libidmefv2_linked_object_t *obj) 
{
        libidmefv2_list_add_tail(head, &obj->_list);
}


static inline void libidmefv2_linked_object_set_id(libidmefv2_linked_object_t *obj, unsigned int id)
{
        obj->_object_id = id;
}


static inline unsigned int libidmefv2_linked_object_get_id(libidmefv2_linked_object_t *obj)
{
        return obj->_object_id;
}



#define libidmefv2_linked_object_get_object(object)  \
        (void *) libidmefv2_list_entry(object, libidmefv2_linked_object_t, _list)


#endif /* _LIBIDMEFV2_LIBIDMEFV2_LINKED_OBJECT_H */

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

#include "idmefv2.h"
#include "idmefv2-object-prv.h"
#include "idmefv2-object.h"
#include "idmefv2-class.h"


typedef struct idmefv2_linked_object idmefv2_linked_object_t;



idmefv2_class_id_t idmefv2_object_get_class(idmefv2_object_t *obj)
{
        return obj->_idmefv2_object_id;
}



idmefv2_object_t *idmefv2_object_ref(idmefv2_object_t *obj)
{
        int ret;

        ret = idmefv2_class_ref(obj->_idmefv2_object_id, obj);
        libidmefv2_return_val_if_fail(ret == 0, NULL);

        return obj;
}



void idmefv2_object_destroy(idmefv2_object_t *obj)
{
        idmefv2_class_destroy(obj->_idmefv2_object_id, obj);
}



int idmefv2_object_compare(idmefv2_object_t *obj1, idmefv2_object_t *obj2)
{
        if ( obj1->_idmefv2_object_id != obj2->_idmefv2_object_id )
                return -1;

        return idmefv2_class_compare(obj1->_idmefv2_object_id, obj1, obj2);
}



int idmefv2_object_clone(idmefv2_object_t *obj, idmefv2_object_t **dst)
{
        return idmefv2_class_clone(obj->_idmefv2_object_id, obj, (void **) dst);
}



int idmefv2_object_copy(idmefv2_object_t *src, idmefv2_object_t *dst)
{
        return idmefv2_class_copy(src->_idmefv2_object_id, src, dst);
}



int idmefv2_object_print(idmefv2_object_t *obj, libidmefv2_io_t *fd)
{
        return idmefv2_class_print(obj->_idmefv2_object_id, obj, fd);
}

int idmefv2_object_print_json(idmefv2_object_t *obj, libidmefv2_io_t *fd)
{
        return idmefv2_class_print_json(obj->_idmefv2_object_id, obj, fd);
}

int idmefv2_object_print_binary(idmefv2_object_t *obj, libidmefv2_io_t *fd)
{
        return idmefv2_class_print_binary(obj->_idmefv2_object_id, obj, fd);
}

void idmefv2_object_add(libidmefv2_list_t *head, idmefv2_object_t *object)
{
        libidmefv2_return_if_fail(idmefv2_class_is_listed(object->_idmefv2_object_id));
        libidmefv2_list_add(head, &((idmefv2_linked_object_t *) object)->_list);
}


void idmefv2_object_add_tail(libidmefv2_list_t *head, idmefv2_object_t *object)
{
        libidmefv2_return_if_fail(idmefv2_class_is_listed(object->_idmefv2_object_id));
        libidmefv2_list_add_tail(head, &((idmefv2_linked_object_t *) object)->_list);
}


void idmefv2_object_del(idmefv2_object_t *object)
{
        libidmefv2_return_if_fail(idmefv2_class_is_listed(object->_idmefv2_object_id));
        libidmefv2_list_del(&((idmefv2_linked_object_t *) object)->_list);
}


void idmefv2_object_del_init(idmefv2_object_t *object)
{
        libidmefv2_return_if_fail(idmefv2_class_is_listed(object->_idmefv2_object_id));
        libidmefv2_list_del(&((idmefv2_linked_object_t *) object)->_list);
        libidmefv2_list_init(&((idmefv2_linked_object_t *) object)->_list);
}


void *idmefv2_object_get_list_entry(libidmefv2_list_t *elem)
{
        return libidmefv2_list_entry(elem, idmefv2_linked_object_t, _list);
}

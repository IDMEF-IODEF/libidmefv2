/*****
*
* Copyright (C) 2009-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
*
* This file is part of the LibIdmefv2 library.
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

#include <iostream>

#include <string.h>
#include <idmefv2.h>
#include <idmefv2-path.h>
#include <idmefv2-message-print.h>
#include <idmefv2-object-prv.h>

#include "libidmefv2-error.hxx"
#include "idmefv2-path.hxx"
#include "idmefv2.hxx"

using namespace Idmefv2;


IDMEFV2::IDMEFV2() : _object(NULL)
{
        int ret;

        ret = idmefv2_message_new((idmefv2_message_t **) &_object);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2::IDMEFV2(const char *json) : _object(NULL)
{
        int ret;

        ret = idmefv2_object_new_from_json(&_object, json);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2::IDMEFV2(const IDMEFV2 &idmefv2)
{
        _object = (idmefv2._object) ? idmefv2_object_ref(idmefv2._object) : NULL;
}


IDMEFV2::IDMEFV2(idmefv2_object_t *object)
{
        _object = object;
}


IDMEFV2::~IDMEFV2()
{
        if ( _object )
                idmefv2_object_destroy(_object);
}

void IDMEFV2::set(const char *path, const std::vector<IDMEFV2> &value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, IDMEFV2 *value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, const std::vector<IDMEFV2Value> &value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, IDMEFV2Value *value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, IDMEFV2Value &value)
{
        IDMEFV2Path(*this, path).set(*this, &value);
}


void IDMEFV2::set(const char *path, const std::string &value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, const char *value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, int32_t value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, int64_t value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, uint64_t value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, float value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, double value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


void IDMEFV2::set(const char *path, IDMEFV2Time &value)
{
        IDMEFV2Path(*this, path).set(*this, value);
}


IDMEFV2Value IDMEFV2::get(const char *path)
{
        return IDMEFV2Path(*this, path).get(*this);
}


int IDMEFV2::operator == (const IDMEFV2 *idmefv2)
{
        if ( ! idmefv2 )
                return 0;

        if ( _object == idmefv2->_object )
                return 1;

        return idmefv2_object_compare(_object, idmefv2->_object) == 0 ? 1 : 0;
}


IDMEFV2 IDMEFV2::clone()
{
        int ret;
        idmefv2_object_t *clone;

        ret = idmefv2_object_clone(_object, &clone);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return IDMEFV2(clone);
}


idmefv2_class_id_t IDMEFV2::getId() const
{
        return _object->_idmefv2_object_id;
}


const std::string IDMEFV2::toString() const
{
        int ret;
        std::string str;
        libidmefv2_io_t *fd;

        ret = libidmefv2_io_new(&fd);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        libidmefv2_io_set_buffer_io(fd);
        idmefv2_object_print(_object, fd);

        str.assign((const char *) libidmefv2_io_get_fdptr(fd), libidmefv2_io_pending(fd));

        libidmefv2_io_close(fd);
        libidmefv2_io_destroy(fd);

        return str;
}


const std::string IDMEFV2::toJSON() const
{
        int ret;
        std::string str;
        libidmefv2_io_t *fd;

        ret = libidmefv2_io_new(&fd);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        libidmefv2_io_set_buffer_io(fd);
        idmefv2_object_print_json(_object, fd);

        str.assign((const char *) libidmefv2_io_get_fdptr(fd), libidmefv2_io_pending(fd));

        libidmefv2_io_close(fd);
        libidmefv2_io_destroy(fd);

        return str;
}

const std::string IDMEFV2::toBinary() const
{
        int ret;
        std::string str;
        libidmefv2_io_t *fd;

        ret = libidmefv2_io_new(&fd);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        libidmefv2_io_set_buffer_io(fd);
        idmefv2_object_print_binary(_object, fd);

        str.assign((const char *) libidmefv2_io_get_fdptr(fd), libidmefv2_io_pending(fd));

        libidmefv2_io_close(fd);
        libidmefv2_io_destroy(fd);

        return str;
}


IDMEFV2::operator const std::string() const
{
        return toString();
}


IDMEFV2::operator idmefv2_object_t *() const
{
        return (idmefv2_object_t *) _object;
}


IDMEFV2 &IDMEFV2::operator = (const IDMEFV2 &idmefv2)
{
        if ( this != &idmefv2 && _object != idmefv2._object ) {
                if ( _object )
                        idmefv2_object_destroy(_object);

                _object = (idmefv2._object) ? idmefv2_object_ref(idmefv2._object) : NULL;
        }

        return *this;
}

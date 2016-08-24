/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann@libidmefv2-ids.com>
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

#include "idmefv2.hxx"
#include "idmefv2-path.hxx"
#include "idmefv2-class.hxx"
#include "libidmefv2-error.hxx"

#include "idmefv2-object-prv.h"

using namespace Idmefv2;


IDMEFV2Path::IDMEFV2Path(const char *buffer)
{
        int ret;

        ret = idmefv2_path_new_fast(&_path, buffer);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}



IDMEFV2Path::IDMEFV2Path(IDMEFV2 &idmefv2, const char *buffer)
{
        int ret;
        idmefv2_object_t *obj = (idmefv2_object_t *) idmefv2;

        ret = idmefv2_path_new_from_root_fast(&_path, obj->_idmefv2_object_id, buffer);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}



IDMEFV2Path::IDMEFV2Path(idmefv2_path_t *path)
{
        _path = path;
}


IDMEFV2Path::IDMEFV2Path(const IDMEFV2Path &path)
{
        _path = (path._path) ? idmefv2_path_ref(path._path) : NULL;
}


IDMEFV2Path::~IDMEFV2Path()
{
        idmefv2_path_destroy(_path);
}


IDMEFV2Value IDMEFV2Path::get(const IDMEFV2 &message) const
{
        int ret;
        idmefv2_value_t *value;

        ret = idmefv2_path_get(_path, message, &value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        else if ( ret == 0 )
                return IDMEFV2Value((idmefv2_value_t *) NULL);

        return IDMEFV2Value(value);
}



Idmefv2::IDMEFV2Value::IDMEFV2ValueTypeEnum IDMEFV2Path::getValueType(int depth) const
{
        return (Idmefv2::IDMEFV2Value::IDMEFV2ValueTypeEnum) idmefv2_path_get_value_type(_path, depth);
}


int IDMEFV2Path::checkOperator(idmefv2_criterion_operator_t op) const
{
        return idmefv2_path_check_operator(_path, op);
}



idmefv2_criterion_operator_t IDMEFV2Path::getApplicableOperators() const
{
        int ret;
        idmefv2_criterion_operator_t res;

        ret = idmefv2_path_get_applicable_operators(_path, &res);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return res;
}



void IDMEFV2Path::set(IDMEFV2 &message, const std::vector<IDMEFV2> &value) const
{
        int ret;
        IDMEFV2Value v = value;

        ret = idmefv2_path_set(_path, message, v);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, IDMEFV2 *value) const
{
        int ret;

        if ( ! value )
                ret = idmefv2_path_set(_path, message, NULL);
        else
                ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));

        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, IDMEFV2Value *value) const
{
        int ret;

        if ( ! value )
                ret = idmefv2_path_set(_path, message, NULL);
        else
                ret = idmefv2_path_set(_path, message, *value);

        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, const std::vector<IDMEFV2Value> &value) const
{
        int ret;
        IDMEFV2Value v = value;

        ret = idmefv2_path_set(_path, message, v);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, IDMEFV2Value &value) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, const std::string &value) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, const char *value) const
{
        int ret;

        if ( value )
                ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));
        else
                ret = idmefv2_path_set(_path, message, (idmefv2_value_t *) NULL);

        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}



void IDMEFV2Path::set(IDMEFV2 &message, int32_t value) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}



void IDMEFV2Path::set(IDMEFV2 &message, int64_t value) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, uint64_t value) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, float value) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, double value) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, IDMEFV2Value(value));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Path::set(IDMEFV2 &message, IDMEFV2Time &time) const
{
        int ret;

        ret = idmefv2_path_set(_path, message, IDMEFV2Value(time));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}



IDMEFV2Class IDMEFV2Path::getClass(int depth) const
{
        return IDMEFV2Class(idmefv2_path_get_class(_path, depth));
}


int IDMEFV2Path::setIndex(unsigned int index, int depth)
{
        if ( depth < 0 )
                depth = getDepth();

        return idmefv2_path_set_index(_path, depth, index);
}


int IDMEFV2Path::undefineIndex(int depth)
{
        if ( depth < 0 )
                depth = getDepth();

        return idmefv2_path_undefine_index(_path, depth);
}


int IDMEFV2Path::getIndex(int depth) const
{
        if ( depth < 0 )
                depth = getDepth();

        return idmefv2_path_get_index(_path, depth);
}


int IDMEFV2Path::makeChild(const char *child_name, unsigned int index=0)
{
        return idmefv2_path_make_child(_path, child_name, index);
}



int IDMEFV2Path::makeParent()
{
        return idmefv2_path_make_parent(_path);
}


int IDMEFV2Path::compare(IDMEFV2Path *path, int depth) const
{
        int ret;

        if ( depth < 0 )
                ret = idmefv2_path_compare(_path, path->_path);
        else
                ret = idmefv2_path_ncompare(_path, path->_path, depth);

        return ret;
}


IDMEFV2Path IDMEFV2Path::clone() const
{
        int ret;
        idmefv2_path_t *cpath;

        ret = idmefv2_path_clone(_path, &cpath);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return IDMEFV2Path(cpath);
}


const char *IDMEFV2Path::getName(int depth) const
{
        return idmefv2_path_get_name(_path, depth);
}


bool IDMEFV2Path::isAmbiguous() const
{
        return idmefv2_path_is_ambiguous(_path);
}


int IDMEFV2Path::hasLists() const
{
        return idmefv2_path_has_lists(_path);
}


bool IDMEFV2Path::isList(int depth) const
{
        return idmefv2_path_is_list(_path, depth);
}


unsigned int IDMEFV2Path::getDepth() const
{
        return idmefv2_path_get_depth(_path);
}


IDMEFV2Path &IDMEFV2Path::operator=(const IDMEFV2Path &path)
{
        if ( this != &path && _path != path._path ) {
                if ( _path )
                        idmefv2_path_destroy(_path);

                _path = (path._path) ? idmefv2_path_ref(path._path) : NULL;
        }

        return *this;
}


IDMEFV2Path::operator idmefv2_path_t *() const
{
        return _path;
}


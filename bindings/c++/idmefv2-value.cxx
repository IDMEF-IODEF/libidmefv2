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

#include <string.h>
#include <sstream>

#include "libidmefv2-error.hxx"
#include "idmefv2.hxx"
#include "idmefv2-value.hxx"
#include "idmefv2-criteria.hxx"

using namespace Idmefv2;


IDMEFV2Value::~IDMEFV2Value()
{
        if ( _value )
                idmefv2_value_destroy(_value);
}


IDMEFV2Value::IDMEFV2ValueTypeEnum IDMEFV2Value::getType() const
{
        return (IDMEFV2Value::IDMEFV2ValueTypeEnum) idmefv2_value_get_type(_value);
}


bool IDMEFV2Value::isNull() const
{
        return (! _value) ? TRUE : FALSE;
}


IDMEFV2Value::IDMEFV2Value()
{
        _value = NULL;
}


IDMEFV2Value::IDMEFV2Value(const IDMEFV2Value &value)
{
        idmefv2_value_t *v = NULL;

        if ( value._value )
                v = idmefv2_value_ref(value._value);

        _value = v;
}


void IDMEFV2Value::_InitFromString(const char *value, size_t len)
{
        int ret;
        libidmefv2_string_t *str;

        ret = libidmefv2_string_new_dup_fast(&str, value, len);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        ret = idmefv2_value_new_string(&_value, str);
        if ( ret < 0 ) {
                libidmefv2_string_destroy(str);
                throw LibIdmefv2Error(ret);
        }
}


IDMEFV2Value::IDMEFV2Value(const char *value)
{
        _InitFromString(value, strlen(value));
}


IDMEFV2Value::IDMEFV2Value(const std::string &value)
{
        _InitFromString(value.c_str(), value.size());
}


IDMEFV2Value::IDMEFV2Value(int32_t value)
{
        int ret = idmefv2_value_new_int32(&_value, value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Value::IDMEFV2Value(int64_t value)
{
        int ret = idmefv2_value_new_int64(&_value, value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Value::IDMEFV2Value(uint64_t value)
{
        int ret = idmefv2_value_new_uint64(&_value, value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Value::IDMEFV2Value(float value)
{
        int ret = idmefv2_value_new_float(&_value, value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Value::IDMEFV2Value(double value)
{
        int ret = idmefv2_value_new_double(&_value, value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Value::IDMEFV2Value(IDMEFV2Time &time)
{
        int ret = idmefv2_value_new_time(&_value, idmefv2_time_ref(time));
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Value::IDMEFV2Value(IDMEFV2 *idmefv2)
{
        int ret;
        idmefv2_value_t *v = NULL;

        if (  idmefv2 ) {
                ret = idmefv2_value_new_class(&v, idmefv2->getId(), idmefv2_object_ref((idmefv2_object_t *) *idmefv2));
                if ( ret < 0 )
                       throw LibIdmefv2Error(ret);
        }

        _value = v;
}


IDMEFV2Value::IDMEFV2Value(const std::vector<IDMEFV2> &value)
{
        int ret;
        idmefv2_value_t *vitem;
        std::vector<Idmefv2::IDMEFV2>::const_iterator i;

        ret = idmefv2_value_new_list(&_value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        for ( i = value.begin(); i != value.end(); i++ ) {
                ret = idmefv2_value_new_class(&vitem, i->getId(), idmefv2_object_ref((idmefv2_object_t *) *i));
                if ( ret < 0 )
                        throw LibIdmefv2Error(ret);

                idmefv2_value_list_add(_value, vitem);
        }
}


IDMEFV2Value::IDMEFV2Value(const std::vector<IDMEFV2Value> &value)
{
        int ret;
        std::vector<Idmefv2::IDMEFV2Value>::const_iterator i;

        ret = idmefv2_value_new_list(&_value);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        for ( i = value.begin(); i != value.end(); i++ )
                idmefv2_value_list_add(_value, idmefv2_value_ref(*i));
}

IDMEFV2Value::IDMEFV2Value(idmefv2_value_t *value)
{
        _value = value;
}


int IDMEFV2Value::match(const IDMEFV2Value &value, int op) const
{
        int ret;

        ret = idmefv2_value_match(this->_value, value._value, (idmefv2_criterion_operator_t) op);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return ret;
}


IDMEFV2Value IDMEFV2Value::clone() const
{
        int ret;
        idmefv2_value_t *clone;

        ret = idmefv2_value_clone(_value, &clone);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return IDMEFV2Value(clone);
}


const std::string IDMEFV2Value::toString() const
{
        int ret;
        std::string s;
        libidmefv2_string_t *str;

        ret = libidmefv2_string_new(&str);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        ret = idmefv2_value_to_string(_value, str);
        if ( ret < 0 ) {
                libidmefv2_string_destroy(str);
                throw LibIdmefv2Error(ret);
        }

        s = libidmefv2_string_get_string(str);
        libidmefv2_string_destroy(str);

        return s;
}


static int iterate_cb(idmefv2_value_t *value, void *extra)
{
        std::vector<IDMEFV2Value> *vlist = (std::vector<IDMEFV2Value> *) extra;

        if ( value )
                value = idmefv2_value_ref(value);

        vlist->push_back(IDMEFV2Value(value));

        return 0;
}


IDMEFV2Value::operator std::vector<IDMEFV2Value> () const
{
        std::vector<IDMEFV2Value> vlist;

        if ( ! _value )
                return vlist;

        if ( getType() != TYPE_LIST ) {
                std::stringstream s;
                s << "Left value doesn't fit '" << idmefv2_value_type_to_string((idmefv2_value_type_id_t) getType()) << "' requirement";
                throw LibIdmefv2Error(s.str());
        }

        idmefv2_value_iterate(_value, iterate_cb, &vlist);

        return vlist;
}


IDMEFV2Value::operator IDMEFV2Time () const
{
        libidmefv2_except_if_fail(_value);

        if ( getType() != TYPE_TIME ) {
                std::stringstream s;
                s << "Left value doesn't fit '" << idmefv2_value_type_to_string((idmefv2_value_type_id_t) getType()) << "' requirement";
                throw LibIdmefv2Error(s.str());
        }

        return IDMEFV2Time(idmefv2_time_ref(idmefv2_value_get_time(_value)));
}



IDMEFV2Value::operator int32_t () const
{
        IDMEFV2ValueTypeEnum vtype;

        libidmefv2_except_if_fail(_value);
        vtype = getType();

        if ( vtype == TYPE_INT8 )
                return idmefv2_value_get_int8(_value);

        else if ( vtype == TYPE_UINT8 )
                return idmefv2_value_get_uint8(_value);

        else if ( vtype == TYPE_INT16 )
                return idmefv2_value_get_int16(_value);

        else if ( vtype == TYPE_UINT16 )
                return idmefv2_value_get_uint16(_value);

        else if ( vtype == TYPE_INT32 )
                return idmefv2_value_get_int32(_value);

        else if ( vtype == TYPE_ENUM )
                return idmefv2_value_get_enum(_value);

        std::stringstream s;
        s << "Left value doesn't fit '" << idmefv2_value_type_to_string((idmefv2_value_type_id_t) vtype) << "' requirement";
        throw LibIdmefv2Error(s.str());
}


IDMEFV2Value::operator uint32_t () const
{
        libidmefv2_except_if_fail(_value);

        if ( getType() == TYPE_UINT32 )
                return idmefv2_value_get_uint32(_value);
        else
                return (int32_t) *this;
}


IDMEFV2Value::operator int64_t () const
{
        libidmefv2_except_if_fail(_value);

        if ( getType() == TYPE_INT64 )
                return idmefv2_value_get_int64(_value);
        else
                return (uint32_t) *this;
}


IDMEFV2Value::operator uint64_t () const
{
        libidmefv2_except_if_fail(_value);

        if ( getType() == TYPE_UINT64 )
                return idmefv2_value_get_uint64(_value);
        else
                return (int64_t) *this;
}


IDMEFV2Value::operator float () const
{
        IDMEFV2ValueTypeEnum vtype;

        libidmefv2_except_if_fail(_value);
        vtype = getType();

        if ( vtype == TYPE_FLOAT )
                return idmefv2_value_get_float(_value);

        else if ( vtype == TYPE_DATA ) {
                idmefv2_data_t *d = idmefv2_value_get_data(_value);

                if ( idmefv2_data_get_type(d) == IDMEFV2_DATA_TYPE_FLOAT )
                        return idmefv2_data_get_float(d);
        }

        std::stringstream s;
        s << "Left value doesn't fit '" << idmefv2_value_type_to_string((idmefv2_value_type_id_t) vtype) << "' requirement";
        throw LibIdmefv2Error(s.str());
}


IDMEFV2Value::operator double () const
{
        libidmefv2_except_if_fail(_value);

        if ( getType() == TYPE_DOUBLE )
                return idmefv2_value_get_double(_value);
        else
                return (float) *this;
}


std::string IDMEFV2Value::convert_string() const
{
        std::stringstream s;
        libidmefv2_except_if_fail(_value);

        if ( getType() == TYPE_STRING )
                return libidmefv2_string_get_string(idmefv2_value_get_string(_value));

        else if ( getType() == TYPE_TIME )
                return IDMEFV2Time(idmefv2_time_ref(idmefv2_value_get_time(_value)));

        else if ( getType() == TYPE_ENUM )
                return idmefv2_class_enum_to_string(idmefv2_value_get_class(_value), idmefv2_value_get_enum(_value));

        else if ( getType() == TYPE_DATA ) {
                idmefv2_data_t *d = idmefv2_value_get_data(_value);
                idmefv2_data_type_t t = idmefv2_data_get_type(d);

                if ( t == IDMEFV2_DATA_TYPE_CHAR_STRING )
                        return (const char *) idmefv2_data_get_char_string(d);

                else if ( t == IDMEFV2_DATA_TYPE_CHAR ) {
                        s << idmefv2_data_get_char(d);
                        return s.str();
                }

                else if ( t == IDMEFV2_DATA_TYPE_FLOAT ) {
                        s << idmefv2_data_get_float(d);
                        return s.str();
                }

                else if ( t == IDMEFV2_DATA_TYPE_INT ) {
                        s << idmefv2_data_get_int(d);
                        return s.str();
                }

                else {
                        s << "Left value doesn't fit 'data' type '" << t << "' requirement";
                        throw LibIdmefv2Error(s.str());
                }
        }

        s << "Left value doesn't fit '" << idmefv2_value_type_to_string((idmefv2_value_type_id_t) getType()) << "' requirement";
        throw LibIdmefv2Error(s.str());
}


IDMEFV2Value &IDMEFV2Value::operator=(const IDMEFV2Value &p)
{
        if ( this != &p && _value != p._value ) {
                if ( _value )
                        idmefv2_value_destroy(_value);

                _value = (p._value) ? idmefv2_value_ref(p._value) : NULL;
        }

        return *this;
}


IDMEFV2Value::operator const char*() const
{
        static std::string t;
        t = this->convert_string();
        return t.c_str();
}


IDMEFV2Value::operator idmefv2_value_t *() const
{
        return _value;
}


bool IDMEFV2Value::operator <= (const IDMEFV2Value &value) const
{
        return this->match(value, IDMEFV2Criterion::OPERATOR_LESSER|IDMEFV2Criterion::OPERATOR_EQUAL);
}


bool IDMEFV2Value::operator >= (const IDMEFV2Value &value) const
{
        return this->match(value, IDMEFV2Criterion::OPERATOR_GREATER|IDMEFV2Criterion::OPERATOR_EQUAL);
}


bool IDMEFV2Value::operator < (const IDMEFV2Value &value) const
{
        return this->match(value, IDMEFV2Criterion::OPERATOR_LESSER);
}


bool IDMEFV2Value::operator > (const IDMEFV2Value &value) const
{
        return this->match(value, IDMEFV2Criterion::OPERATOR_GREATER);
}


bool IDMEFV2Value::operator == (const IDMEFV2Value &value) const
{
        return this->match(value, IDMEFV2Criterion::OPERATOR_EQUAL);
}


bool IDMEFV2Value::operator == (const std::vector<IDMEFV2Value> &vlist) const
{
        return this->match(vlist, IDMEFV2Criterion::OPERATOR_EQUAL);
}


bool IDMEFV2Value::operator != (const IDMEFV2Value &value) const
{
        return this->match(value, IDMEFV2Criterion::OPERATOR_NOT|IDMEFV2Criterion::OPERATOR_EQUAL);
}

/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
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

#include <string>
#include <idmefv2.h>
#include <idmefv2-value-type.h>

#include "libidmefv2.hxx"


using namespace Idmefv2;



IDMEFV2Class::IDMEFV2Class(idmefv2_class_id_t id)
{
        _id = id;
        _depth = 0;
}


IDMEFV2Class::IDMEFV2Class(const IDMEFV2Path &path)
{
        int i;
        IDMEFV2Class root;

        for ( i = 0; i < path.getDepth(); i++ )
                root = root.get(path.getName(i));

        *this = root;
}



IDMEFV2Class::IDMEFV2Class(const std::string &path)
{
        *this = IDMEFV2Class(IDMEFV2Path(path.c_str()));
}


IDMEFV2Class::IDMEFV2Class(IDMEFV2Class &parent, int child_id, int depth)
{
        IDMEFV2Class::IDMEFV2ClassElem elem;

        if ( depth >= 16 )
                throw LibIdmefv2Error(libidmefv2_error(LIBIDMEFV2_ERROR_IDMEFV2_PATH_DEPTH));

        _depth = depth;
        _pathelem = parent._pathelem;
        _id = idmefv2_class_get_child_class(parent._id, child_id);

        elem.parent_id = parent._id;
        elem.idx = child_id;
        _pathelem.push_back(elem);
}


std::string IDMEFV2Class::toString(void)
{
        unsigned int i = 0;
        std::string s  = "IDMEFV2Class(" + getName();

        do {
                if ( i > 0 )
                        s += ", ";

                try {
                        s += get(i++).toString();
                } catch(...) {
                        break;
                }
        } while ( TRUE );

        s += "\n)";

        return s;
}


bool IDMEFV2Class::isList(void)
{
        if ( _pathelem.size() == 0 )
                throw LibIdmefv2Error("Already in rootclass, cannot retrieve parents info");

        return idmefv2_class_is_child_list(_pathelem.back().parent_id, _pathelem.back().idx);
}


bool IDMEFV2Class::isKeyedList(void)
{
        if ( _pathelem.size() == 0 )
                throw LibIdmefv2Error("Already in rootclass, cannot retrieve parents info");

        return idmefv2_class_is_child_keyed_list(_pathelem.back().parent_id, _pathelem.back().idx);
}


Idmefv2::IDMEFV2Value::IDMEFV2ValueTypeEnum IDMEFV2Class::getValueType(void)
{
        if ( _pathelem.size() == 0 )
                throw LibIdmefv2Error("Already in rootclass, cannot retrieve parents info");

        return (Idmefv2::IDMEFV2Value::IDMEFV2ValueTypeEnum) idmefv2_class_get_child_value_type(_pathelem.back().parent_id, _pathelem.back().idx);
}


std::string IDMEFV2Class::getName(void)
{
        if ( _pathelem.size() == 0 )
                return idmefv2_class_get_name(_id);

        return idmefv2_class_get_child_name(_pathelem.back().parent_id, _pathelem.back().idx);
}


size_t IDMEFV2Class::getDepth(void)
{
        return _pathelem.size();
}


std::string IDMEFV2Class::getPath(int rootidx, int depth, const std::string &sep, const std::string &listidx)
{
        std::string out;

        if ( depth >= 0 ) {
                if ( (depth + 1) == _pathelem.size() )
                        return getName();

                IDMEFV2ClassElem elem = _pathelem[depth];
                return idmefv2_class_get_child_name(elem.parent_id, elem.idx);
        }

        for ( std::vector<IDMEFV2ClassElem>::iterator it = _pathelem.begin() + rootidx; it != _pathelem.end(); it++) {
                out += idmefv2_class_get_child_name((*it).parent_id, (*it).idx);

                if ( idmefv2_class_is_child_list((*it).parent_id, (*it).idx) )
                        out += listidx;

                if ( it + 1 != _pathelem.end() )
                        out += sep;
        }

        return out;
}



IDMEFV2Class IDMEFV2Class::get(const std::string &name)
{
        int i = idmefv2_class_find_child(_id, name.c_str());

        if ( i < 0 )
                throw LibIdmefv2Error(i);

        return get(i);
}


IDMEFV2Class IDMEFV2Class::get(int i)
{
        idmefv2_class_id_t cl;
        idmefv2_value_type_id_t vl;

        cl = idmefv2_class_get_child_class(_id, i);
        if ( cl < 0 ) {
                vl = (idmefv2_value_type_id_t) idmefv2_class_get_child_value_type(_id, i);
                if ( vl < 0 )
                        throw LibIdmefv2Error(vl);
        }

        return IDMEFV2Class(*this, i, _depth + 1);
}



std::vector<std::string> IDMEFV2Class::getEnumValues(void)
{
        int i = 0;
        const char *ret;
        std::vector<std::string> ev;

        if ( getValueType() != IDMEFV2Value::TYPE_ENUM )
                throw LibIdmefv2Error("Input class is not enumeration");

        do {
                ret = idmefv2_class_enum_to_string(_id, i++);
                if ( ret )
                        ev.push_back(ret);

        } while ( ret || i == 1 ); /* entry 0 might be NULL, if the enumeration has no default value */

        return ev;
}



IDMEFV2Criterion::IDMEFV2CriterionOperatorEnum IDMEFV2Class::getApplicableOperator(void)
{
        int ret;
        idmefv2_criterion_operator_t op;

        ret = idmefv2_value_type_get_applicable_operators((idmefv2_value_type_id_t) getValueType(), &op);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return (IDMEFV2Criterion::IDMEFV2CriterionOperatorEnum) ret;
}

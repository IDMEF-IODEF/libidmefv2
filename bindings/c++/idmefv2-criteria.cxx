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
#include "idmefv2-criteria.hxx"
#include "libidmefv2-error.hxx"

using namespace Idmefv2;

IDMEFV2Criteria::~IDMEFV2Criteria()
{
        if ( _criteria )
                idmefv2_criteria_destroy(_criteria);
}


IDMEFV2Criteria::IDMEFV2Criteria(const IDMEFV2Criteria &criteria)
{
        _criteria = (criteria._criteria) ? idmefv2_criteria_ref(criteria._criteria) : NULL;
}


IDMEFV2Criteria::IDMEFV2Criteria(idmefv2_criteria_t *criteria)
{
        _criteria = criteria;
}


IDMEFV2Criteria::IDMEFV2Criteria(const char *criteria)
{
        int ret;

        ret = idmefv2_criteria_new_from_string(&_criteria, criteria);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Criteria::IDMEFV2Criteria(const std::string &criteria)
{
        int ret;

        ret = idmefv2_criteria_new_from_string(&_criteria, criteria.c_str());
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Criteria::IDMEFV2Criteria()
{
        int ret;

        ret = idmefv2_criteria_new(&_criteria);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Criteria IDMEFV2Criteria::clone() const
{
        int ret;
        idmefv2_criteria_t *cl;

        ret = idmefv2_criteria_clone(this->_criteria, &cl);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return IDMEFV2Criteria(cl);
}


void IDMEFV2Criteria::andCriteria(const IDMEFV2Criteria &criteria)
{
        idmefv2_criteria_and_criteria(this->_criteria, criteria._criteria);
}


void IDMEFV2Criteria::orCriteria(const IDMEFV2Criteria &criteria)
{
        idmefv2_criteria_or_criteria(this->_criteria, criteria._criteria);
}


int IDMEFV2Criteria::match(IDMEFV2 *message) const
{
        int ret;

        ret = idmefv2_criteria_match(this->_criteria, *message);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return ret;
}


const std::string IDMEFV2Criteria::toString() const
{
        int ret;
        std::string s;
        libidmefv2_string_t *str;

        ret = libidmefv2_string_new(&str);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        ret = idmefv2_criteria_to_string(_criteria, str);
        if ( ret < 0 ) {
                libidmefv2_string_destroy(str);
                throw LibIdmefv2Error(ret);
        }

        s = libidmefv2_string_get_string(str);
        libidmefv2_string_destroy(str);

        return s;
}


IDMEFV2Criteria::operator const std::string() const
{
        return toString();
}



IDMEFV2Criteria::operator idmefv2_criteria_t *() const
{
        return _criteria;
}



IDMEFV2Criteria &IDMEFV2Criteria::operator=(const IDMEFV2Criteria &criteria)
{
        if ( this != &criteria && _criteria != criteria._criteria ) {
                if ( _criteria )
                        idmefv2_criteria_destroy(_criteria);

                _criteria = (criteria._criteria) ? idmefv2_criteria_ref(criteria._criteria) : NULL;
        }

        return *this;
}

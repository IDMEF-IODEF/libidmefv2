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

#include "libidmefv2-error.hxx"
#include "idmefv2-time.hxx"

using namespace Idmefv2;


IDMEFV2Time::~IDMEFV2Time()
{
        if ( _time )
                idmefv2_time_destroy(_time);
}


IDMEFV2Time::IDMEFV2Time(const IDMEFV2Time &time)
{
        _time = (time._time) ? idmefv2_time_ref(time._time) : NULL;
}


IDMEFV2Time::IDMEFV2Time()
{
        int ret;

        ret = idmefv2_time_new_from_gettimeofday(&_time);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

}


IDMEFV2Time::IDMEFV2Time(idmefv2_time_t *time)
{
        _time = time;
}


IDMEFV2Time::IDMEFV2Time(time_t time)
{
        int ret;

        ret = idmefv2_time_new_from_time(&_time, &time);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Time::IDMEFV2Time(const char *str)
{
        int ret;

        ret = idmefv2_time_new_from_string(&_time, str);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


IDMEFV2Time::IDMEFV2Time(const struct timeval *tv)
{
        int ret;

        ret = idmefv2_time_new_from_timeval(&_time, tv);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}



void IDMEFV2Time::set(const time_t *t)
{
        idmefv2_time_set_from_time(_time, t);
}


void IDMEFV2Time::set(const struct timeval *tv)
{
        int ret;

        ret = idmefv2_time_set_from_timeval(_time, tv);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}



void IDMEFV2Time::set(const char *str)
{
        int ret;

        ret = idmefv2_time_set_from_string(_time, str);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Time::set()
{
        int ret;

        ret = idmefv2_time_set_from_gettimeofday(_time);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);
}


void IDMEFV2Time::setSec(uint32_t sec)
{
        idmefv2_time_set_sec(_time, sec);
}


void IDMEFV2Time::setUSec(uint32_t usec)
{
        idmefv2_time_set_usec(_time, usec);
}


void IDMEFV2Time::setGmtOffset(int32_t gmtoff)
{
        idmefv2_time_set_gmt_offset(_time, gmtoff);
}


uint32_t IDMEFV2Time::getSec() const
{
        return idmefv2_time_get_sec(_time);
}


uint32_t IDMEFV2Time::getUSec() const
{
        return idmefv2_time_get_usec(_time);
}


int32_t IDMEFV2Time::getGmtOffset() const
{
        return idmefv2_time_get_gmt_offset(_time);
}


IDMEFV2Time IDMEFV2Time::clone() const
{
        int ret;
        idmefv2_time_t *clone;

        ret = idmefv2_time_clone(_time, &clone);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        return IDMEFV2Time(clone);
}


const std::string IDMEFV2Time::toString() const
{
        int ret;
        std::string cs;
        libidmefv2_string_t *str = NULL;

        ret = libidmefv2_string_new(&str);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        ret = idmefv2_time_to_string(_time, str);
        if ( ret < 0 )
                throw LibIdmefv2Error(ret);

        cs = libidmefv2_string_get_string(str);
        libidmefv2_string_destroy(str);

        return cs;
}


bool IDMEFV2Time::operator <= (const IDMEFV2Time &time) const
{
        return ( (double) *this <= (double) time );
}


bool IDMEFV2Time::operator < (const IDMEFV2Time &time) const
{
        return ( (double) *this < (double) time );
}


bool IDMEFV2Time::operator >= (const IDMEFV2Time &time) const
{
        return ( (double) *this >= (double) time );
}


bool IDMEFV2Time::operator > (const IDMEFV2Time &time) const
{
        return ( (double) *this > (double) time );
}


bool IDMEFV2Time::operator != (const IDMEFV2Time &time) const
{
        return ( (double) *this != (double) time );
}


bool IDMEFV2Time::operator == (const IDMEFV2Time &time) const
{
        return ( (double) *this == (double) time );
}


IDMEFV2Time::operator int() const
{
        return getSec();
}


IDMEFV2Time::operator long() const
{
        return getSec();
}


IDMEFV2Time::operator double() const
{
        return getSec() + (getUSec() * 1e-6);
}


IDMEFV2Time::operator const std::string() const
{
        return toString();
}


IDMEFV2Time::operator idmefv2_time_t *() const
{
        return _time;
}


IDMEFV2Time &IDMEFV2Time::operator=(const IDMEFV2Time &time)
{
        if ( this != &time && _time != time._time ) {
                if ( _time )
                        idmefv2_time_destroy(_time);

                _time = (time._time) ? idmefv2_time_ref(time._time) : NULL;
        }

        return *this;
}


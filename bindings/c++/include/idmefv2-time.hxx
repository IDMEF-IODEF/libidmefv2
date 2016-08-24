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

#ifndef _LIBIDMEFV2_IDMEFV2_TIME_HXX
#define _LIBIDMEFV2_IDMEFV2_TIME_HXX

#include "idmefv2.h"

namespace Idmefv2 {
        class IDMEFV2Time {
            protected:
                idmefv2_time_t *_time;

            public:
                IDMEFV2Time();
                IDMEFV2Time(idmefv2_time_t *time);
                IDMEFV2Time(const time_t time);
                IDMEFV2Time(const char *string);
                IDMEFV2Time(const struct timeval *tv);
                IDMEFV2Time(const IDMEFV2Time &value);
                ~IDMEFV2Time();

                void set();
                void set(const time_t *time);
                void set(const char *string);
                void set(const struct timeval *tv);
                void setSec(uint32_t sec);
                void setUSec(uint32_t usec);
                void setGmtOffset(int32_t gmtoff);

                uint32_t getSec() const;
                uint32_t getUSec() const;
                int32_t getGmtOffset() const;
                double getTime() const { return (double) *this; };

                IDMEFV2Time clone() const;
                const std::string toString() const;

                operator int() const;
                operator long() const;
                operator double() const;
                operator const std::string() const;
                operator idmefv2_time_t *() const;

                bool operator != (const IDMEFV2Time &time) const;
                bool operator >= (const IDMEFV2Time &time) const;
                bool operator <= (const IDMEFV2Time &time) const;
                bool operator == (const IDMEFV2Time &time) const;
                bool operator > (const IDMEFV2Time &time) const;
                bool operator < (const IDMEFV2Time &time) const;

                IDMEFV2Time & operator = (const IDMEFV2Time &p);
        };
};

#endif

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

#ifndef _LIBIDMEFV2_IDMEFV2_VALUE_HXX
#define _LIBIDMEFV2_IDMEFV2_VALUE_HXX

#include <vector>
#include "idmefv2-time.hxx"

namespace Idmefv2
{
        class IDMEFV2;
}

namespace Idmefv2 {
        class IDMEFV2Value {
            private:
                void _InitFromString(const char *value, size_t size);
                std::string convert_string() const;

            protected:
                idmefv2_value_t *_value;
                std::string _myconv;

            public:
                enum IDMEFV2ValueTypeEnum {
                        TYPE_UNKNOWN    = IDMEFV2_VALUE_TYPE_UNKNOWN,
                        TYPE_INT8       = IDMEFV2_VALUE_TYPE_INT8,
                        TYPE_UINT8      = IDMEFV2_VALUE_TYPE_UINT8,
                        TYPE_INT16      = IDMEFV2_VALUE_TYPE_INT16,
                        TYPE_UINT16     = IDMEFV2_VALUE_TYPE_UINT16,
                        TYPE_INT32      = IDMEFV2_VALUE_TYPE_INT32,
                        TYPE_UINT32     = IDMEFV2_VALUE_TYPE_UINT32,
                        TYPE_INT64      = IDMEFV2_VALUE_TYPE_INT64,
                        TYPE_UINT64     = IDMEFV2_VALUE_TYPE_UINT64,
                        TYPE_FLOAT      = IDMEFV2_VALUE_TYPE_FLOAT,
                        TYPE_DOUBLE     = IDMEFV2_VALUE_TYPE_DOUBLE,
                        TYPE_STRING     = IDMEFV2_VALUE_TYPE_STRING,
                        TYPE_TIME       = IDMEFV2_VALUE_TYPE_TIME,
                        TYPE_DATA       = IDMEFV2_VALUE_TYPE_DATA,
                        TYPE_ENUM       = IDMEFV2_VALUE_TYPE_ENUM,
                        TYPE_LIST       = IDMEFV2_VALUE_TYPE_LIST,
                        TYPE_CLASS      = IDMEFV2_VALUE_TYPE_CLASS
                };

                IDMEFV2ValueTypeEnum getType() const;
                bool isNull() const;

                IDMEFV2Value();
                ~IDMEFV2Value();
                IDMEFV2Value(IDMEFV2 *idmefv2);
                IDMEFV2Value(const std::vector<IDMEFV2> &value);
                IDMEFV2Value(const IDMEFV2Value &value);
                IDMEFV2Value(const std::vector<IDMEFV2Value> &value);
                IDMEFV2Value(idmefv2_value_t *value);
                IDMEFV2Value(const std::string &value);
                IDMEFV2Value(const char *value);
                IDMEFV2Value(int32_t value);
                IDMEFV2Value(int64_t value);
                IDMEFV2Value(uint64_t value);
                IDMEFV2Value(float value);
                IDMEFV2Value(double value);
                IDMEFV2Value(Idmefv2::IDMEFV2Time &time);

                int match(const IDMEFV2Value &value, int op) const;
                IDMEFV2Value clone() const;
                const std::string toString() const;

                operator int32_t() const;
                operator uint32_t() const;
                operator int64_t() const;
                operator uint64_t() const;
                operator double() const;
                operator std::vector<IDMEFV2Value>() const;
                operator const char*() const;
                operator float() const;
                operator Idmefv2::IDMEFV2Time() const;
                operator idmefv2_value_t *() const;

                IDMEFV2Value & operator=(const IDMEFV2Value &p);

                bool operator == (const std::vector<IDMEFV2Value> &vlist) const;
                bool operator <= (const IDMEFV2Value &value) const;
                bool operator >= (const IDMEFV2Value &value) const;
                bool operator < (const IDMEFV2Value &value) const;
                bool operator > (const IDMEFV2Value &value) const;
                bool operator == (const IDMEFV2Value &value) const;
                bool operator != (const IDMEFV2Value &value) const;
        };
};

#endif

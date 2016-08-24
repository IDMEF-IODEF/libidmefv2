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

#ifndef _LIBIDMEFV2_IDMEFV2_HXX
#define _LIBIDMEFV2_IDMEFV2_HXX

#include <string>
#include <iostream>
#include "idmefv2-value.hxx"
#include "idmefv2-time.hxx"

namespace Idmefv2
{
        class IDMEFV2;
}

namespace Idmefv2 {
        class IDMEFV2 {
            protected:
                idmefv2_object_t *_object;

            public:
                ~IDMEFV2();
                IDMEFV2();
                IDMEFV2(const char *json);
                IDMEFV2(const IDMEFV2 &idmefv2);
                IDMEFV2(idmefv2_object_t *message);

                void set(const char *path, const std::vector<Idmefv2::IDMEFV2> &value);
                void set(const char *path, Idmefv2::IDMEFV2 *value);
                void set(const char *path, const std::vector<Idmefv2::IDMEFV2Value> &value);
                void set(const char *path, const std::string &value);
                void set(const char *path, Idmefv2::IDMEFV2Value *value);
                void set(const char *path, Idmefv2::IDMEFV2Value &value);
                void set(const char *path, Idmefv2::IDMEFV2Time &value);
                void set(const char *path, const char *value);
                void set(const char *path, int32_t value);
                void set(const char *path, int64_t value);
                void set(const char *path, uint64_t value);
                void set(const char *path, float value);
                void set(const char *path, double value);
                Idmefv2::IDMEFV2Value get(const char *path);


                IDMEFV2 clone();
                idmefv2_class_id_t getId() const;
                const std::string toString() const;
                const std::string toJSON() const;
                const std::string toBinary() const;

                operator const std::string() const;
                operator idmefv2_object_t *() const;
                IDMEFV2 &operator = (const IDMEFV2 &idmefv2);
                int operator == (const IDMEFV2 *idmefv2);
        };
};

#endif /* __LIBIDMEFV2_IDMEFV2_HXX */

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

#ifndef _LIBIDMEFV2_IDMEFV2_PATH_HXX
#define _LIBIDMEFV2_IDMEFV2_PATH_HXX

#include "idmefv2.h"
#include "idmefv2-path.h"

#include "idmefv2.hxx"
#include "libidmefv2-error.hxx"
#include "idmefv2-value.hxx"


namespace Idmefv2 {
        class IDMEFV2Class;

        class IDMEFV2Path {
            private:
                idmefv2_path_t *_path;

            public:
                IDMEFV2Path(const char *buffer);
                IDMEFV2Path(Idmefv2::IDMEFV2 &idmefv2, const char *buffer);

                IDMEFV2Path(idmefv2_path_t *path);
                IDMEFV2Path(const IDMEFV2Path &path);
                ~IDMEFV2Path();

                Idmefv2::IDMEFV2Value get(const Idmefv2::IDMEFV2 &message) const;
                void set(Idmefv2::IDMEFV2 &message, const std::vector<Idmefv2::IDMEFV2> &value) const;
                void set(Idmefv2::IDMEFV2 &message, Idmefv2::IDMEFV2 *value) const;
                void set(Idmefv2::IDMEFV2 &message, const std::vector<Idmefv2::IDMEFV2Value> &value) const;
                void set(Idmefv2::IDMEFV2 &message, Idmefv2::IDMEFV2Value *value) const;
                void set(Idmefv2::IDMEFV2 &message, Idmefv2::IDMEFV2Value &value) const;
                void set(Idmefv2::IDMEFV2 &message, Idmefv2::IDMEFV2Time &time) const;
                void set(Idmefv2::IDMEFV2 &message, const std::string &value) const;
                void set(Idmefv2::IDMEFV2 &message, const char *value) const;
                void set(Idmefv2::IDMEFV2 &message, int32_t value) const;
                void set(Idmefv2::IDMEFV2 &message, int64_t value) const;
                void set(Idmefv2::IDMEFV2 &message, uint64_t value) const;
                void set(Idmefv2::IDMEFV2 &message, float value) const;
                void set(Idmefv2::IDMEFV2 &message, double value) const;

                Idmefv2::IDMEFV2Class getClass(int depth=-1) const;
                Idmefv2::IDMEFV2Value::IDMEFV2ValueTypeEnum getValueType(int depth=-1) const;
                int setIndex(unsigned int index, int depth=-1);
                int undefineIndex(int depth=-1);
                int getIndex(int depth=-1) const;
                int makeChild(const char *child_name, unsigned int index);
                int makeParent();
                int compare(IDMEFV2Path *path, int depth=-1) const;
                IDMEFV2Path clone() const;

                int checkOperator(idmefv2_criterion_operator_t op) const;
                idmefv2_criterion_operator_t getApplicableOperators() const;

                const char *getName(int depth=-1) const;
                bool isAmbiguous() const;
                int hasLists() const;
                bool isList(int depth=-1) const;
                unsigned int getDepth() const;

                IDMEFV2Path &operator = (const IDMEFV2Path &path);
                operator idmefv2_path_t*() const;
        };
};

#endif

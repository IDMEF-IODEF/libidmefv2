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

#ifndef _LIBIDMEFV2_IDMEFV2_CLASS_HXX
#define _LIBIDMEFV2_IDMEFV2_CLASS_HXX

#include <string>
#include <vector>

#include "idmefv2-criteria.hxx"
#include "idmefv2-path.hxx"

namespace Idmefv2 {
        class IDMEFV2Class {
            private:
                class IDMEFV2ClassElem {
                        public:
                                int idx;
                                idmefv2_class_id_t parent_id;
                };

                int _depth;
                idmefv2_class_id_t _id;
                std::vector<IDMEFV2Class::IDMEFV2ClassElem> _pathelem;

                IDMEFV2Class(IDMEFV2Class &parent, int child_id, int depth=0);
            public:
                IDMEFV2Class(idmefv2_class_id_t id=IDMEFV2_CLASS_ID_MESSAGE);
                IDMEFV2Class(const IDMEFV2Path &path);
                IDMEFV2Class(const std::string &path);

                size_t getDepth(void);
                IDMEFV2Class get(int child);
                IDMEFV2Class get(const std::string &name);

                size_t getChildCount() { return idmefv2_class_get_child_count(_id); };

                /* main object operation */
                bool isList(void);
                bool isKeyedList(void);
                std::string getName(void);
                std::string toString(void);
                Idmefv2::IDMEFV2Value::IDMEFV2ValueTypeEnum getValueType(void);
                std::string getPath(int rootidx=0, int depth=-1, const std::string &sep = ".", const std::string &listidx="");
                std::vector<std::string> getEnumValues(void);
                Idmefv2::IDMEFV2Criterion::IDMEFV2CriterionOperatorEnum getApplicableOperator(void);
        };
};

#endif

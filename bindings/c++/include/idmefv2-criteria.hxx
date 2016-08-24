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

#ifndef _LIBIDMEFV2_IDMEFV2_CRITERIA_HXX
#define _LIBIDMEFV2_IDMEFV2_CRITERIA_HXX

#include <string>
#include "idmefv2.h"
#include "idmefv2.hxx"

namespace Idmefv2 {
        class IDMEFV2Criterion {
            public:
                enum IDMEFV2CriterionOperatorEnum {
                        OPERATOR_NOT     = IDMEFV2_CRITERION_OPERATOR_NOT,
                        OPERATOR_NOCASE  = IDMEFV2_CRITERION_OPERATOR_NOCASE,

                        OPERATOR_EQUAL   = IDMEFV2_CRITERION_OPERATOR_EQUAL,
                        OPERATOR_EQUAL_NOCASE = IDMEFV2_CRITERION_OPERATOR_EQUAL_NOCASE,
                        OPERATOR_NOT_EQUAL = IDMEFV2_CRITERION_OPERATOR_NOT_EQUAL,
                        OPERATOR_NOT_EQUAL_NOCASE = IDMEFV2_CRITERION_OPERATOR_NOT_EQUAL_NOCASE,

                        OPERATOR_LESSER = IDMEFV2_CRITERION_OPERATOR_LESSER,
                        OPERATOR_LESSER_OR_EQUAL = IDMEFV2_CRITERION_OPERATOR_LESSER_OR_EQUAL,

                        OPERATOR_GREATER = IDMEFV2_CRITERION_OPERATOR_GREATER,
                        OPERATOR_GREATER_OR_EQUAL = IDMEFV2_CRITERION_OPERATOR_GREATER_OR_EQUAL,

                        OPERATOR_SUBSTR = IDMEFV2_CRITERION_OPERATOR_SUBSTR,
                        OPERATOR_SUBSTR_NOCASE = IDMEFV2_CRITERION_OPERATOR_SUBSTR_NOCASE,
                        OPERATOR_NOT_SUBSTR = IDMEFV2_CRITERION_OPERATOR_NOT_SUBSTR,
                        OPERATOR_NOT_SUBSTR_NOCASE = IDMEFV2_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE,

                        OPERATOR_REGEX = IDMEFV2_CRITERION_OPERATOR_REGEX,
                        OPERATOR_REGEX_NOCASE = IDMEFV2_CRITERION_OPERATOR_REGEX_NOCASE,
                        OPERATOR_NOT_REGEX = IDMEFV2_CRITERION_OPERATOR_NOT_REGEX,
                        OPERATOR_NOT_REGEX_NOCASE = IDMEFV2_CRITERION_OPERATOR_NOT_REGEX_NOCASE,

                        OPERATOR_NULL = IDMEFV2_CRITERION_OPERATOR_NULL,
                        OPERATOR_NOT_NULL = IDMEFV2_CRITERION_OPERATOR_NOT_NULL
                };
        };

        class IDMEFV2Criteria {
            private:
                idmefv2_criteria_t *_criteria;

            public:
                ~IDMEFV2Criteria();
                IDMEFV2Criteria();
                IDMEFV2Criteria(const IDMEFV2Criteria &criteria);
                IDMEFV2Criteria(const char *criteria);
                IDMEFV2Criteria(const std::string &criteria);
                IDMEFV2Criteria(idmefv2_criteria_t *criteria);

                int match(Idmefv2::IDMEFV2 *message) const;
                IDMEFV2Criteria clone() const;
                void andCriteria(const IDMEFV2Criteria &criteria);
                void orCriteria(const IDMEFV2Criteria &criteria);
                const std::string toString() const;

                operator const std::string() const;
                operator idmefv2_criteria_t *() const;
                IDMEFV2Criteria &operator=(const IDMEFV2Criteria &criteria);
        };
};

#endif

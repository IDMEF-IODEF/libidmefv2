/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann@prelude-ids.com>
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

#ifndef _LIBIDMEFV2_LIBIDMEFV2_ERROR_HXX
#define _LIBIDMEFV2_LIBIDMEFV2_ERROR_HXX

#include <string>
#include <exception>

#define libidmefv2_except_if_fail(cond) do {                                   \
        if ( ! (cond) )                                                     \
                throw LibIdmefv2Error(libidmefv2_error(LIBIDMEFV2_ERROR_ASSERTION)); \
} while(0)


namespace Idmefv2 {
        class LibIdmefv2Error: public std::exception {
            protected:
                int _error;
                std::string _message;

            public:
                virtual ~LibIdmefv2Error() throw() {};
                LibIdmefv2Error(void) throw();
                LibIdmefv2Error(int error) throw();
                LibIdmefv2Error(const std::string &message) throw();

                int getCode(void) const;

                virtual const char *what() const throw();
                operator const char *() const;
                operator int () const;
        };
};

#endif

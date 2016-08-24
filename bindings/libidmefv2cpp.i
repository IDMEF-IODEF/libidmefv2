/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@libidmefv2-ids.com>
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

#if defined(SWIGPYTHON) || defined(SWIGLUA)
%module idmefv2
#else
%module Idmefv2
#endif

%warnfilter(509);
%feature("nothread", "1");

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%{
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <list>
#include <sstream>

#include "libidmefv2.hxx"
#include "libidmefv2-error.hxx"
#include "idmefv2-criteria.hxx"
#include "idmefv2-value.hxx"
#include "idmefv2-path.hxx"
#include "idmefv2-time.hxx"
#include "idmefv2.hxx"

using namespace Idmefv2;
%}


typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

%ignore libidmefv2_error_t;
typedef signed int libidmefv2_error_t;

%ignore idmefv2_class_id_t;
typedef int idmefv2_class_id_t;

typedef long long time_t;


%exception {
        try {
                $action
        } catch(Idmefv2::LibIdmefv2Error &e) {
                SWIG_exception(SWIG_RuntimeError, e.what());
                SWIG_fail;
        }
}


#ifdef SWIGPERL
%include perl/libidmefv2cpp-perl.i
#endif

#ifdef SWIGPYTHON
%include python/libidmefv2cpp-python.i
#endif

#ifdef SWIGRUBY
%include ruby/libidmefv2cpp-ruby.i
#endif

#ifdef SWIGLUA
%include lua/libidmefv2cpp-lua.i
#endif


%ignore operator <<(std::ostream &os, const Idmefv2::IDMEFV2 &idmefv2);
%ignore operator >>(std::istream &is, const Idmefv2::IDMEFV2 &idmefv2);


%template() std::vector<std::string>;
%template() std::vector<Idmefv2::IDMEFV2>;
%template() std::vector<Idmefv2::IDMEFV2Value>;
%template() std::vector<Idmefv2::Connection>;

#ifdef SWIG_COMPILE_LIBIDMEFV2
%extend Idmefv2::IDMEFV2 {
        Idmefv2::IDMEFV2Value get(const char *path) {
                Idmefv2::IDMEFV2Value value;
                Idmefv2::IDMEFV2Path ipath = Idmefv2::IDMEFV2Path(*self, path);

                value = ipath.get(*self);
                if ( value.isNull() && ipath.isAmbiguous() ) {
                        std::vector<Idmefv2::IDMEFV2Value> v;
                        return Idmefv2::IDMEFV2Value(v);
                }

                return value;
        }
}

%extend Idmefv2::IDMEFV2Path {
        Idmefv2::IDMEFV2Value get(Idmefv2::IDMEFV2 &message) {
                Idmefv2::IDMEFV2Value value;

                value = self->get(message);
                if ( value.isNull() && self->isAmbiguous() ) {
                        std::vector<Idmefv2::IDMEFV2Value> v;
                        return Idmefv2::IDMEFV2Value(v);
                }

                return value;
        }
}
#endif

%ignore Idmefv2::IDMEFV2::get;
%ignore Idmefv2::IDMEFV2Path::get;


%fragment("SWIG_FromBytePtrAndSize", "header", fragment="SWIG_FromCharPtrAndSize") %{
#ifndef SWIG_FromBytePtrAndSize
# define SWIG_FromBytePtrAndSize(arg, len) SWIG_FromCharPtrAndSize(arg, len)
#endif
%}


%fragment("IDMEFV2Value_to_SWIG", "header", fragment="SWIG_From_double",
                                          fragment="SWIG_From_float",
                                          fragment="SWIG_From_int", fragment="SWIG_From_unsigned_SS_int",
                                          fragment="SWIG_From_long_SS_long", fragment="SWIG_From_unsigned_SS_long_SS_long",
                                          fragment="SWIG_FromCharPtr", fragment="SWIG_FromCharPtrAndSize", fragment="SWIG_FromBytePtrAndSize",
                                          fragment="IDMEFV2ValueList_to_SWIG") {

int IDMEFV2Value_to_SWIG(TARGET_LANGUAGE_SELF self, const Idmefv2::IDMEFV2Value &result, void *extra, TARGET_LANGUAGE_OUTPUT_TYPE ret)
{
        idmefv2_value_t *value = result;
        Idmefv2::IDMEFV2Value::IDMEFV2ValueTypeEnum type = result.getType();

        if ( type == Idmefv2::IDMEFV2Value::TYPE_STRING ) {
                libidmefv2_string_t *str = idmefv2_value_get_string(value);
                *ret = SWIG_FromCharPtrAndSize(libidmefv2_string_get_string(str), libidmefv2_string_get_len(str));
        }

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_INT8 )
                *ret = SWIG_From_int(idmefv2_value_get_int8(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_UINT8 )
                *ret = SWIG_From_unsigned_SS_int(idmefv2_value_get_uint8(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_INT16 )
                *ret = SWIG_From_int(idmefv2_value_get_int16(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_UINT16 )
                *ret = SWIG_From_unsigned_SS_int(idmefv2_value_get_uint16(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_INT32 )
                *ret = SWIG_From_int(idmefv2_value_get_int32(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_UINT32 )
                *ret = SWIG_From_unsigned_SS_int(idmefv2_value_get_uint32(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_INT64 )
                *ret = SWIG_From_long_SS_long(idmefv2_value_get_int64(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_UINT64 )
                *ret = SWIG_From_unsigned_SS_long_SS_long(idmefv2_value_get_uint64(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_FLOAT )
                *ret = SWIG_From_float(idmefv2_value_get_float(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_DOUBLE )
                *ret = SWIG_From_double(idmefv2_value_get_double(value));

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_ENUM ) {
                const char *s = idmefv2_class_enum_to_string(idmefv2_value_get_class(value), idmefv2_value_get_enum(value));
                *ret = SWIG_FromCharPtr(s);
        }

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_TIME ) {
                Idmefv2::IDMEFV2Time t = result;
                *ret = SWIG_NewPointerObj(new Idmefv2::IDMEFV2Time(t), $descriptor(Idmefv2::IDMEFV2Time *), 1);
        }

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_LIST )
                *ret = IDMEFV2ValueList_to_SWIG(self, result, extra);

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_DATA ) {
                idmefv2_data_t *d = idmefv2_value_get_data(value);
                idmefv2_data_type_t t = idmefv2_data_get_type(d);

                if ( t == IDMEFV2_DATA_TYPE_BYTE || t == IDMEFV2_DATA_TYPE_BYTE_STRING )
                        *ret = SWIG_FromBytePtrAndSize((const char *)idmefv2_data_get_data(d), idmefv2_data_get_len(d));

                else if ( t == IDMEFV2_DATA_TYPE_CHAR )
                        *ret = SWIG_FromCharPtrAndSize((const char *)idmefv2_data_get_data(d), idmefv2_data_get_len(d));

                else if ( t == IDMEFV2_DATA_TYPE_CHAR_STRING )
                        *ret = SWIG_FromCharPtrAndSize((const char *)idmefv2_data_get_data(d), idmefv2_data_get_len(d) - 1);

                else if ( t == IDMEFV2_DATA_TYPE_FLOAT )
                        *ret = SWIG_From_float(idmefv2_data_get_float(d));

                else if ( t == IDMEFV2_DATA_TYPE_UINT32 || IDMEFV2_DATA_TYPE_INT )
                        *ret = SWIG_From_unsigned_SS_long_SS_long(idmefv2_data_get_int(d));
        }

        else if ( type == Idmefv2::IDMEFV2Value::TYPE_CLASS ) {
                idmefv2_object_t *obj = (idmefv2_object_t *) idmefv2_value_get_object(value);
                *ret = SWIG_NewPointerObj(new Idmefv2::IDMEFV2(idmefv2_object_ref(obj)), $descriptor(Idmefv2::IDMEFV2 *), 1);
        }

        else return -1;

        return 1;
}
}

%ignore Idmefv2::IDMEFV2Value::operator const char*() const;
%ignore Idmefv2::IDMEFV2Value::operator std::vector<IDMEFV2Value>() const;
%ignore Idmefv2::IDMEFV2Value::operator Idmefv2::IDMEFV2Time() const;
%ignore Idmefv2::IDMEFV2Value::operator int8_t() const;
%ignore Idmefv2::IDMEFV2Value::operator uint8_t() const;
%ignore Idmefv2::IDMEFV2Value::operator int16_t() const;
%ignore Idmefv2::IDMEFV2Value::operator uint16_t() const;
%ignore Idmefv2::IDMEFV2Value::operator int32_t() const;
%ignore Idmefv2::IDMEFV2Value::operator uint32_t() const;
%ignore Idmefv2::IDMEFV2Value::operator int64_t() const;
%ignore Idmefv2::IDMEFV2Value::operator uint64_t() const;
%ignore Idmefv2::IDMEFV2Value::operator float() const;
%ignore Idmefv2::IDMEFV2Value::operator double() const;

/*
 * Force SWIG to use the IDMEFV2Value * version of the Set() function,
 * so that the user might provide NULL IDMEFV2Value. Force usage of the
 * std::string value, for binary data.
 */
%ignore Idmefv2::IDMEFV2::set(char const *, int8_t);
%ignore Idmefv2::IDMEFV2::set(char const *, uint8_t);
%ignore Idmefv2::IDMEFV2::set(char const *, int16_t);
%ignore Idmefv2::IDMEFV2::set(char const *, uint16_t);
%ignore Idmefv2::IDMEFV2::set(char const *, char const *);
%ignore Idmefv2::IDMEFV2::set(char const *, Idmefv2::IDMEFV2Value &value);
%ignore Idmefv2::IDMEFV2Path::set(Idmefv2::IDMEFV2 &, int8_t) const;
%ignore Idmefv2::IDMEFV2Path::set(Idmefv2::IDMEFV2 &, uint8_t) const;
%ignore Idmefv2::IDMEFV2Path::set(Idmefv2::IDMEFV2 &, int16_t) const;
%ignore Idmefv2::IDMEFV2Path::set(Idmefv2::IDMEFV2 &, uint16_t) const;
%ignore Idmefv2::IDMEFV2Path::set(Idmefv2::IDMEFV2 &, char const *) const;
%ignore Idmefv2::IDMEFV2Path::set(Idmefv2::IDMEFV2 &, Idmefv2::IDMEFV2Value &) const;
%ignore Idmefv2::IDMEFV2Value::IDMEFV2Value(char const *);

%ignore idmefv2_path_t;
%ignore idmefv2_object_t;
%ignore idmefv2_criteria_t;
%ignore operator idmefv2_path_t *() const;
%ignore operator idmefv2_criteria_t *() const;
%ignore operator idmefv2_object_t *() const;
%ignore operator idmefv2_message_t *() const;
%ignore operator idmefv2_time_t *() const;
%ignore operator idmefv2_value_t *() const;

/*
 * We need to unlock the interpreter lock before calling certain methods
 * because they might acquire internal libidmefv2 mutex that may also be
 * acquired undirectly through the libidmefv2 asynchronous stack.
 *
 * [Thread 2]: Libidmefv2 async stack
 * -> Lock internal mutexX
 *    -> libidmefv2_log()
 *       -> SWIG/C log callback
 *          -> Wait on Interpreter lock [DEADLOCK]
 *             -> Python logging callback (never called)
 *
 * [Thread 1] ConnectionPool::Recv()
 *  -> Acquire Interpreter lock
 *      *** At this time, thread 2 lock internal mutexX
 *      -> Wait on internal mutexX [DEADLOCK]
 *
 * In this situation, [Thread 1] hold the Interpreter lock and is
 * waiting on mutexX, which itself cannot be released by [Thread 2]
 * until [Thread 1] unlock the Interpreter lock.
 *
 * One rule to prevent deadlock is to always acquire mutex in the same
 * order. We thus need to make sure the interpreter lock is released
 * before calling C++ method that are susceptible to lock a mutex that
 * is also triggered from the asynchronous interface.
 *
 * Note that we are not releasing the Interpreter lock in all C++ call,
 * because it come at a performance cost, so we only try to do it when
 * needed.
 */

#ifdef SWIG_COMPILE_LIBIDMEFV2
%feature("exceptionclass") Idmefv2::LibIdmefv2Error;
%feature("kwargs") Idmefv2::IDMEFV2Class::getPath;
%feature("kwargs") Idmefv2::IDMEFV2Path::getClass;
%feature("kwargs") Idmefv2::IDMEFV2Path::getValueType;
%feature("kwargs") Idmefv2::IDMEFV2Path::setIndex;
%feature("kwargs") Idmefv2::IDMEFV2Path::getIndex;
%feature("kwargs") Idmefv2::IDMEFV2Path::undefineIndex;
%feature("kwargs") Idmefv2::IDMEFV2Path::compare;
%feature("kwargs") Idmefv2::IDMEFV2Path::getName;
%feature("kwargs") Idmefv2::IDMEFV2Path::isList;

%include libidmefv2.hxx
%include libidmefv2-error.hxx
%include idmefv2-criteria.hxx
%include idmefv2-value.hxx
%include idmefv2-path.hxx
%include idmefv2-time.hxx
%include idmefv2-class.hxx
%include idmefv2.hxx
#endif

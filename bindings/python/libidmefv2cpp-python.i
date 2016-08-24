/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libidmefv2-ids.com>
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

%begin %{
#define TARGET_LANGUAGE_SELF PyObject *
#define TARGET_LANGUAGE_OUTPUT_TYPE PyObject **
%}

%include std_list.i

%ignore *::operator int;
%ignore *::operator long;
%ignore *::operator double;
%ignore *::operator const char *;
%ignore *::operator const std::string;
%ignore *::operator ();

%feature("python:slot", "tp_str", functype="reprfunc") *::what;
%feature("python:slot", "tp_repr", functype="reprfunc") *::toString;
%feature("python:slot", "mp_subscript", functype="binaryfunc") *::get;
%feature("python:slot", "mp_ass_subscript", functype="objobjargproc") *::set;
%feature("python:slot", "tp_hash") Idmefv2::IDMEFV2Value::getType;

/*
 * IDMEFV2Class
 */
%feature("python:slot", "tp_str", functype="reprfunc") Idmefv2::IDMEFV2Class::getName;
%feature("python:slot", "sq_item", functype="ssizeargfunc") Idmefv2::IDMEFV2Class::_get2;
%feature("python:slot", "mp_subscript", functype="binaryfunc") Idmefv2::IDMEFV2Class::get;
%feature("python:slot", "mp_length", functype="lenfunc") Idmefv2::IDMEFV2Class::getChildCount;

/*
 * IDMEFV2Time
 */
%feature("python:slot", "nb_int", functype="unaryfunc") Idmefv2::IDMEFV2Time::getSec;
%feature("python:slot", "nb_long", functype="unaryfunc") Idmefv2::IDMEFV2Time::_getSec2;
%feature("python:slot", "nb_float", functype="unaryfunc") Idmefv2::IDMEFV2Time::getTime;

/*
 *
 */
%feature("python:slot", "nb_lshift") Idmefv2::IDMEFV2::readExcept;
%feature("python:slot", "nb_rshift") Idmefv2::IDMEFV2::write;


%ignore *::operator =;


%fragment("SWIG_FromBytePtrAndSize", "header", fragment="SWIG_FromCharPtrAndSize") %{
#if PY_VERSION_HEX < 0x03000000
# define SWIG_FromBytePtrAndSize(arg, len) PyString_FromStringAndSize(arg, len)
#else
# define SWIG_FromBytePtrAndSize(arg, len) PyBytes_FromStringAndSize(arg, len)
#endif
%}

/* tell squid not to cast void * value */
%typemap(in) void *nocast_file_p %{
#if PY_VERSION_HEX < 0x03000000
        if ( !PyFile_Check((PyObject *) $input) ) {
                const char *errstr = "Argument is not a file object.";
                PyErr_SetString(PyExc_RuntimeError, errstr);
                return NULL;
        }
#else
        extern PyTypeObject PyIOBase_Type;
        if ( ! PyObject_IsInstance((PyObject *) $input, (PyObject *) &PyIOBase_Type) ) {
                const char *errstr = "Argument is not a file object.";
                PyErr_SetString(PyExc_RuntimeError, errstr);
                return NULL;
        }
#endif

        $1 = $input;
%}


%exception {
        try {
                $action
        } catch(Idmefv2::LibIdmefv2Error &e) {
                SWIG_Python_Raise(SWIG_NewPointerObj(new LibIdmefv2Error(e),
                                                     SWIGTYPE_p_Idmefv2__LibIdmefv2Error, SWIG_POINTER_OWN),
                                  "LibIdmefv2Error", SWIGTYPE_p_Idmefv2__LibIdmefv2Error);
                SWIG_fail;
        }
}


#ifdef SWIG_COMPILE_LIBIDMEFV2

%{
typedef PyObject SwigPyObjectState;
%}

/*
 * This is called on Idmefv2::IDMEFV2::__getstate__()
 * Store our internal IDMEFV2 data in the PyObjet __dict__
 */
%typemap(out) SwigPyObjectState * {
        int ret;
        PyObject *state;
        SwigPyObject *pyobj = (SwigPyObject *) self;

        state = PyDict_New();
        if ( ! state ) {
                Py_XDECREF(result);
                SWIG_fail;
        }

        ret = PyDict_SetItemString(state, "__idmefv2_data__", result);
        Py_DECREF(result);

        if ( pyobj->dict ) {
                ret = PyDict_Update(state, pyobj->dict);
                if ( ret < 0 ) {
                        Py_XDECREF(state);
                        SWIG_fail;
                }
        }

        if ( ret < 0 )
                throw LibIdmefv2Error("error setting internal __idmefv2_data__ key");

        $result = state;
}

/*
 * This typemap specifically intercept the call to Idmefv2::IDMEFV2::__setstate__,
 * since at that time (when unpickling), the object __init__ method has not been
 * called, and the underlying Idmefv2::IDMEFV2 object is thus NULL.
 *
 * We manually call tp_init() to handle the underlying object creation here.
 */
%typemap(arginit) (PyObject *state) {
        int ret;
        PyObject *obj;
        static PyTypeObject *pytype = NULL;

        if ( ! pytype ) {
                swig_type_info *sti = SWIG_TypeQuery("Idmefv2::IDMEFV2 *");
                if ( ! sti )
                        throw LibIdmefv2Error("could not find type SWIG type info for 'Idmefv2::IDMEFV2'");

                pytype = ((SwigPyClientData *) sti->clientdata)->pytype;
        }

        obj = PyTuple_New(0);
        ret = pytype->tp_init(self, obj, NULL);
        Py_DECREF(obj);

        if ( ret < 0 )
                throw LibIdmefv2Error("error calling Idmefv2::IDMEFV2 tp_init()");

}


/*
 *
 */
%exception Idmefv2::IDMEFV2::__setstate__ {
        try {
                SwigPyObject *pyobj = (SwigPyObject *) self;

                $function

                /*
                 * This is called at unpickling time, and set our PyObject internal dict.
                 */
                pyobj->dict = arg2;
                Py_INCREF(arg2);
        } catch(Idmefv2::LibIdmefv2Error &e) {
                SWIG_Python_Raise(SWIG_NewPointerObj(new LibIdmefv2Error(e),
                                                     SWIGTYPE_p_Idmefv2__LibIdmefv2Error, SWIG_POINTER_OWN),
                                  "LibIdmefv2Error", SWIGTYPE_p_Idmefv2__LibIdmefv2Error);
                SWIG_fail;
        }
}


/*
 * When a comparison operator is called, this prevent an exception
 * if the compared operand does not have the correct datatype.
 *
 * By returning Py_NotImplemented, the python code might provide its
 * own comparison method within the compared operand class
 */
%typemap(in) (const Idmefv2::IDMEFV2Time &time) {
        int ret;
        void *obj;

        ret = SWIG_ConvertPtr($input, &obj, $descriptor(Idmefv2::IDMEFV2Time *),  0  | 0);
        if ( ! SWIG_IsOK(ret) || ! obj ) {
                Py_INCREF(Py_NotImplemented);
                return Py_NotImplemented;
        }

        $1 = reinterpret_cast< Idmefv2::IDMEFV2Time * >(obj);
}


/*
 * Workaround SWIG %features bug, which prevent us from applying multiple
 * features to the same method.
 */
%extend Idmefv2::IDMEFV2Time {
        long _getSec2(void) {
                return self->getSec();
        }
}

%exception Idmefv2::IDMEFV2Class::_get2 {
        try {
                $action;
        } catch(Idmefv2::LibIdmefv2Error &e) {
                if ( e.getCode() == LIBIDMEFV2_ERROR_IDMEFV2_CLASS_UNKNOWN_CHILD ||
                     e.getCode() == LIBIDMEFV2_ERROR_IDMEFV2_PATH_DEPTH )
                        SWIG_exception_fail(SWIG_IndexError, e.what());
        }
}

%extend Idmefv2::IDMEFV2Class {
        Idmefv2::IDMEFV2Class _get2(int i) {
                return self->get(i);
        }
}
#endif

%fragment("IDMEFV2ValueList_to_SWIG", "header", fragment="IDMEFV2Value_to_SWIG") {
int IDMEFV2Value_to_SWIG(TARGET_LANGUAGE_SELF self, const Idmefv2::IDMEFV2Value &result, void *extra, TARGET_LANGUAGE_OUTPUT_TYPE ret);

PyObject *IDMEFV2ValueList_to_SWIG(TARGET_LANGUAGE_SELF self, const Idmefv2::IDMEFV2Value &value, void *extra)
{
        int j = 0, ret;
        PyObject *pytuple;
        std::vector<Idmefv2::IDMEFV2Value> result = value;
        std::vector<Idmefv2::IDMEFV2Value>::const_iterator i;

        pytuple = PyTuple_New(result.size());

        for ( i = result.begin(); i != result.end(); i++ ) {
                PyObject *val;

                if ( (*i).isNull() ) {
                        Py_INCREF(Py_None);
                        val = Py_None;
                } else {
                        ret = IDMEFV2Value_to_SWIG(self, *i, NULL, &val);
                        if ( ret < 0 )
                                return NULL;
                }

                PyTuple_SetItem(pytuple, j++, val);
        }

        return pytuple;
}
}


%typemap(out, fragment="IDMEFV2Value_to_SWIG") Idmefv2::IDMEFV2Value {
        int ret;

        if ( $1.isNull() ) {
                Py_INCREF(Py_None);
                $result = Py_None;
        } else {
#ifdef SWIGPYTHON_BUILTIN
                ret = IDMEFV2Value_to_SWIG(self, $1, NULL, &$result);
#else
                ret = IDMEFV2Value_to_SWIG(NULL, $1, NULL, &$result);
#endif
                if ( ret < 0 ) {
                        std::string s = "IDMEFV2Value typemap does not handle value of type '";
                        s += idmefv2_value_type_to_string((idmefv2_value_type_id_t) $1.getType());
                        s += "'";
                        SWIG_exception_fail(SWIG_ValueError, s.c_str());
                }
        }
};




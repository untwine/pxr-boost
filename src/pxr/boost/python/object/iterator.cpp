// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include "./iterator_core.hpp"
#include "./function_object.hpp"
#include "../type_list.hpp"

namespace pxr { namespace boost { namespace python { namespace objects {

namespace
{
  PyObject* identity(PyObject* args_, PyObject*)
  {
      PyObject* x = PyTuple_GET_ITEM(args_,0);
      Py_INCREF(x);
      return x;
  }
}

PXR_BOOST_PYTHON_DECL object const& identity_function()
{
    static object result(
        function_object(
            py_function(&identity, python::type_list<PyObject*,PyObject*>())
        )
    );
    return result;
}

void stop_iteration_error()
{
    PyErr_SetObject(PyExc_StopIteration, Py_None);
    throw_error_already_set();
}

}}}} // namespace pxr::boost::python::objects

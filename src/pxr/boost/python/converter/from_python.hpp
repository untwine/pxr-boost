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

#ifndef PXR_BOOST_PYTHON_CONVERTER_FROM_PYTHON_HPP
# define PXR_BOOST_PYTHON_CONVERTER_FROM_PYTHON_HPP

# include "../detail/prefix.hpp"
# include "./rvalue_from_python_data.hpp"

namespace pxr { namespace boost { namespace python { namespace converter {

struct registration;


PXR_BOOST_PYTHON_DECL void* get_lvalue_from_python(
    PyObject* source, registration const&);

PXR_BOOST_PYTHON_DECL bool implicit_rvalue_convertible_from_python(
    PyObject* source, registration const&);

PXR_BOOST_PYTHON_DECL rvalue_from_python_stage1_data rvalue_from_python_stage1(
    PyObject* source, registration const&);

PXR_BOOST_PYTHON_DECL void* rvalue_from_python_stage2(
    PyObject* source, rvalue_from_python_stage1_data&, registration const&);

PXR_BOOST_PYTHON_DECL void* rvalue_result_from_python(
    PyObject*, rvalue_from_python_stage1_data&);

PXR_BOOST_PYTHON_DECL void* reference_result_from_python(PyObject*, registration const&);
PXR_BOOST_PYTHON_DECL void* pointer_result_from_python(PyObject*, registration const&);

PXR_BOOST_PYTHON_DECL void void_result_from_python(PyObject*);

PXR_BOOST_PYTHON_DECL void throw_no_pointer_from_python(PyObject*, registration const&);
PXR_BOOST_PYTHON_DECL void throw_no_reference_from_python(PyObject*, registration const&);

}}}} // namespace pxr::boost::python::converter

#endif // PXR_BOOST_PYTHON_CONVERTER_FROM_PYTHON_HPP

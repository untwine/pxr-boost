// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2004.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include "./tuple.hpp"

namespace pxr { namespace boost { namespace python { namespace detail {

detail::new_reference tuple_base::call(object const& arg_)
{
    return (detail::new_reference)PyObject_CallFunction(
        (PyObject*)&PyTuple_Type, const_cast<char*>("(O)"), 
        arg_.ptr());
}
    
tuple_base::tuple_base()
    : object(detail::new_reference(PyTuple_New(0)))
{}
    
tuple_base::tuple_base(object_cref sequence)
    : object(call(sequence))
{}

static struct register_tuple_pytype_ptr
{
    register_tuple_pytype_ptr()
    {
        const_cast<converter::registration &>(
            converter::registry::lookup(pxr::boost::python::type_id<pxr::boost::python::tuple>())
            ).m_class_object = &PyTuple_Type;
    }
}register_tuple_pytype_ptr_;


}}}}  // namespace pxr::boost::python

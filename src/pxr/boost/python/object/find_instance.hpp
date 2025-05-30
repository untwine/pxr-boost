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

#ifndef PXR_BOOST_PYTHON_OBJECT_FIND_INSTANCE_HPP
# define PXR_BOOST_PYTHON_OBJECT_FIND_INSTANCE_HPP

# include "../type_id.hpp"

namespace pxr { namespace boost { namespace python { namespace objects {

// Given a type_id, find the instance data which corresponds to it, or
// return 0 in case no such type is held.  If null_shared_ptr_only is
// true and the type being sought is a shared_ptr, only find an
// instance if it turns out to be NULL.  Needed for shared_ptr rvalue
// from_python support.
PXR_BOOST_PYTHON_DECL void* find_instance_impl(PyObject*, type_info, bool null_shared_ptr_only = false);

}}}} // namespace pxr::boost::python::objects

#endif // PXR_BOOST_PYTHON_OBJECT_FIND_INSTANCE_HPP

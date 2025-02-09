// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_TYPE_LIST_HPP
#define PXR_BOOST_PYTHON_TYPE_LIST_HPP

#include "pxr/boost/python/common.hpp"

#include "pxr/boost/python/detail/type_list.hpp"

namespace PXR_BOOST_NAMESPACE { namespace python {

template <class ...T>
using type_list = detail::type_list<T...>;

}} // namespace PXR_BOOST_NAMESPACE::python

#endif // PXR_BOOST_PYTHON_TYPE_LIST_HPP

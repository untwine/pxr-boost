// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_DETAIL_GET_POINTER_HPP
#define PXR_BOOST_PYTHON_DETAIL_GET_POINTER_HPP

#include "pxr/boost/python/common.hpp"

#include <memory>

namespace PXR_BOOST_NAMESPACE { namespace python { namespace detail {

template <class T>
T* get_pointer(T* p)
{
    return p;
}

template <class T> 
T* get_pointer(std::unique_ptr<T> const& p)
{
    return p.get();
}

template <class T>
T* get_pointer(std::shared_ptr<T> const& p)
{
    return p.get();
}

}}} // namespace PXR_BOOST_NAMESPACE::python::detail

#endif // PXR_BOOST_PYTHON_DETAIL_GET_POINTER_HPP

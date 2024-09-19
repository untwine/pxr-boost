//
// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
# ifndef PXR_EXTERNAL_BOOST_PYTHON_DETAIL_TYPE_LIST_IMPL_HPP
#  define PXR_EXTERNAL_BOOST_PYTHON_DETAIL_TYPE_LIST_IMPL_HPP

#include "pxr/pxr.h"
#include "pxr/external/boost/python/common.hpp"

#ifndef PXR_USE_INTERNAL_BOOST_PYTHON
#include <boost/python/detail/type_list_impl.hpp>
#else

#  include "pxr/external/boost/python/detail/type_list.hpp"

namespace PXR_BOOST_NAMESPACE { namespace python { namespace detail { 

template <class... T>
struct type_list
    : mpl::vector<T...>
{
};

}}} // namespace PXR_BOOST_NAMESPACE::python::detail

#endif // PXR_USE_INTERNAL_BOOST_PYTHON
# endif // PXR_EXTERNAL_BOOST_PYTHON_DETAIL_TYPE_LIST_IMPL_HPP

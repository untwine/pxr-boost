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

# ifndef PXR_BOOST_PYTHON_DETAIL_TYPE_LIST_IMPL_HPP
#  define PXR_BOOST_PYTHON_DETAIL_TYPE_LIST_IMPL_HPP

#  include "./type_list.hpp"

namespace pxr { namespace boost { namespace python { namespace detail {

template <class... T>
struct type_list
{
    using type = type_list<T...>;
};

}}}} // namespace pxr::boost::python::detail

# endif // PXR_BOOST_PYTHON_DETAIL_TYPE_LIST_IMPL_HPP

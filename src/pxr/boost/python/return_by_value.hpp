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

#ifndef PXR_BOOST_PYTHON_RETURN_BY_VALUE_HPP
# define PXR_BOOST_PYTHON_RETURN_BY_VALUE_HPP

#include "pxr/boost/python/common.hpp"

# include "pxr/boost/python/detail/prefix.hpp"

# include "pxr/boost/python/to_python_value.hpp"
# include "pxr/boost/python/detail/type_traits.hpp"

# include "pxr/boost/python/detail/value_arg.hpp"

namespace PXR_BOOST_NAMESPACE { namespace python { 

struct return_by_value
{
    template <class R>
    struct apply
    {
       typedef to_python_value<
           typename detail::value_arg<R>::type
       > type;
    };
};

}} // namespace PXR_BOOST_NAMESPACE::python

#endif // PXR_BOOST_PYTHON_RETURN_BY_VALUE_HPP

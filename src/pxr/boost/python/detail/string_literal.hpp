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

#ifndef PXR_BOOST_PYTHON_DETAIL_STRING_LITERAL_HPP
# define PXR_BOOST_PYTHON_DETAIL_STRING_LITERAL_HPP

#include "pxr/boost/python/common.hpp"

# include <cstddef>
# include "pxr/boost/python/type.hpp"
# include "pxr/boost/python/detail/type_traits.hpp"
# include "pxr/boost/python/detail/mpl2/bool.hpp"

namespace PXR_BOOST_NAMESPACE { namespace python { namespace detail { 

template <class T>
struct is_string_literal : mpl2::false_
{
};

#  if !defined(__MWERKS__) || __MWERKS__ > 0x2407
template <std::size_t n>
struct is_string_literal<char const[n]> : mpl2::true_
{
};

#  else

// CWPro7 has trouble with the array type deduction above
template <class T, std::size_t n>
struct is_string_literal<T[n]>
    : is_same<T, char const>
{
};
#  endif 

}}} // namespace PXR_BOOST_NAMESPACE::python::detail

#endif // PXR_BOOST_PYTHON_DETAIL_STRING_LITERAL_HPP

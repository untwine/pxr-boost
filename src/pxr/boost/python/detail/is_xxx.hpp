// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_DETAIL_IS_XXX_HPP
# define PXR_BOOST_PYTHON_DETAIL_IS_XXX_HPP

# include <type_traits>

#  define PXR_BOOST_PYTHON_IS_XXX_DEF(name, qualified_name, nargs) \
template <class T>                                                 \
struct is_##name : std::false_type { };                            \
                                                                   \
template <class... T>                                              \
struct is_##name<qualified_name<T...>>                             \
    : std::bool_constant<nargs == sizeof...(T)>                    \
{                                                                  \
};

#endif // PXR_BOOST_PYTHON_DETAIL_IS_XXX_HPP

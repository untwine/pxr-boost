// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_DETAIL_IS_WRAPPER_HPP
# define PXR_BOOST_PYTHON_DETAIL_IS_WRAPPER_HPP

# include "./prefix.hpp"
# include "./mpl2/bool.hpp"

namespace pxr { namespace boost { namespace python {

template <class T> class wrapper;

namespace detail
{
  typedef char (&is_not_wrapper)[2];
  is_not_wrapper is_wrapper_helper(...);
  template <class T>
  char is_wrapper_helper(wrapper<T> const volatile*);

  // A metafunction returning true iff T is [derived from] wrapper<U> 
  template <class T>
  struct is_wrapper
    : mpl2::bool_<(sizeof(detail::is_wrapper_helper((T*)0)) == 1)>
  {};

}}}} // namespace pxr::boost::python::detail

#endif // PXR_BOOST_PYTHON_DETAIL_IS_WRAPPER_HPP

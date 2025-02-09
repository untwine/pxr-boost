// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// (C) Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_TYPE_HPP
# define PXR_BOOST_PYTHON_TYPE_HPP

#ifndef PXR_USE_INTERNAL_BOOST_PYTHON

#include <boost/type.hpp>

namespace pxr { namespace boost { namespace python {

using boost::type;

}}}

#else

namespace pxr { namespace boost { namespace python {

  // Just a simple "type envelope". Useful in various contexts, mostly to work
  // around some MSVC deficiencies.
  template <class T>
  struct type {};

}}}

#endif // PXR_USE_INTERNAL_BOOST_PYTHON
#endif // PXR_BOOST_PYTHON_TYPE_HPP

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

#ifndef PXR_BOOST_PYTHON_SLICE_NIL_HPP
# define PXR_BOOST_PYTHON_SLICE_NIL_HPP

# include "./detail/prefix.hpp"
# include "./object_core.hpp"

namespace pxr { namespace boost { namespace python { namespace api {

class slice_nil : public object
{
 public:
    slice_nil() : object() {}
};

# ifndef _ // Watch out for GNU gettext users, who #define _(x)
static const slice_nil _ = slice_nil();
# endif 

template <class T>
struct slice_bound
{
    typedef object type;
};

template <>
struct slice_bound<slice_nil>
{
    typedef slice_nil type;
};

}

using api::slice_nil;
# ifndef _ // Watch out for GNU gettext users, who #define _(x)
using api::_;
# endif 

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_SLICE_NIL_HPP

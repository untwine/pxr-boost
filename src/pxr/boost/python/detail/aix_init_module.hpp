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

#ifndef PXR_BOOST_PYTHON_DETAIL_AIX_INIT_MODULE_HPP
# define PXR_BOOST_PYTHON_DETAIL_AIX_INIT_MODULE_HPP

# ifdef _AIX
# include "./prefix.hpp"
# include <cstdio>
# ifdef __KCC
#  include <iostream> // this works around a problem in KCC 4.0f
# endif 

namespace pxr { namespace boost { namespace python { namespace detail {

extern "C"
{
    typedef PyObject* (*so_load_function)(char*,char*,FILE*);
}

void aix_init_module(so_load_function, char const* name, void (*init_module)());

}}}} // namespace pxr::boost::python::detail
# endif

#endif // PXR_BOOST_PYTHON_DETAIL_AIX_INIT_MODULE_HPP

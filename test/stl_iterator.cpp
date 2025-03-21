// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright Eric Niebler 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/iterator.hpp>
#include <pxr/boost/python/stl_iterator.hpp>
#include <list>

using namespace pxr::boost::python;

typedef std::list<int> list_int;

void assign(list_int& x, object const& y)
{
    stl_input_iterator<int> begin(y), end;
    x.clear();
    for( ; begin != end; ++begin)
        x.push_back(*begin);
}

PXR_BOOST_PYTHON_MODULE(stl_iterator_ext)
{
    using pxr::boost::python::iterator; // gcc 2.96 bug workaround

    class_<list_int>("list_int")
        .def("assign", assign)
        .def("__iter__", iterator<list_int>())
        ;
}

#include "module_tail.cpp"

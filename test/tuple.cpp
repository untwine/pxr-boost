// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/tuple.hpp>

using namespace pxr::boost::python;

object convert_to_tuple(object data)
{
    return tuple(data);
}

void test_operators(tuple t1, tuple t2, object print)
{
    print(t1 + t2);
}

tuple mktuple0() { return make_tuple(); }
tuple mktuple1(int x) { return make_tuple(x); }
tuple mktuple2(char const* a1, int x) { return make_tuple(a1, x); }

PXR_BOOST_PYTHON_MODULE(tuple_ext)
{
    def("convert_to_tuple",convert_to_tuple);
    def("test_operators",test_operators);
    def("make_tuple", mktuple0);
    def("make_tuple", mktuple1);
    def("make_tuple", mktuple2);
}

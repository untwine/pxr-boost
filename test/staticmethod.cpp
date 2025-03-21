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

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/call_method.hpp>
#include <cassert>

using namespace pxr::boost::python;

struct X
{
    explicit X(int x) : x(x), magic(7654321) { ++counter; }
    X(X const& rhs) : x(rhs.x), magic(7654321) { ++counter; }
    virtual ~X() { assert(magic == 7654321); magic = 6666666; x = 9999; --counter; }

    void set(int _x) { assert(magic == 7654321); this->x = _x; }
    int value() const { assert(magic == 7654321); return x; }
    static int count() { return counter; }
 private:
    void operator=(X const&);
 private:
    int x;
    long magic;
    static int counter;
};
int X::counter;
int getXmagic(){return 7654321;}

PXR_BOOST_PYTHON_MODULE(staticmethod_ext)
{
    class_<X>("X", init<int>())
        .def("value", &X::value)
        .def("set", &X::set)
        .def("count", &X::count)
        .staticmethod("count")
        .def("magic", &getXmagic)
        .staticmethod("magic")
        ;
}

#include "module_tail.cpp"

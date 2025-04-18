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

#include <pxr/boost/python/extract.hpp>
#include <pxr/boost/python/list.hpp>
#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/reference_existing_object.hpp>
#include <pxr/boost/python/return_value_policy.hpp>
#include <pxr/boost/python/implicit.hpp>
#include <string>
#include "test_class.hpp"

using namespace pxr::boost::python;

typedef test_class<> X;

bool extract_bool(object x) { return extract<bool>(x); }

pxr::boost::python::list extract_list(object x)
{
    extract<list> get_list((x));

    // Make sure we always have the right idea about whether it's a list
    bool is_list_1 = get_list.check();
    bool is_list_2 = PyObject_IsInstance(x.ptr(), (PyObject*)&PyList_Type);
    if (is_list_1 != is_list_2) {
        throw std::runtime_error("is_list_1 == is_list_2 failure.");
    }
    return get_list();
}

char const* extract_cstring(object x)
{
    return extract<char const*>(x);
}

std::string extract_string(object x)
{
    std::string s = extract<std::string>(x);
    return s;
}

std::string const& extract_string_cref(object x)
{
    return extract<std::string const&>(x);
}

X extract_X(object x)
{
    return extract<X>(x);
}

X* extract_X_ptr(object x) { return extract<X*>(x); }

X& extract_X_ref(object x)
{
    extract<X&> get_x(x);
    return get_x;
}

int double_X(object n)
{
    extract<X> x(n);
    return x().value() + x().value();
}

bool check_bool(object x) { return extract<bool>(x).check(); }
bool check_list(object x) { return extract<list>(x).check(); }
bool check_cstring(object x) { return extract<char const*>(x).check(); }
bool check_string(object x) { return extract<std::string>(x).check(); }
bool check_string_cref(object x) { return extract<std::string const&>(x).check(); }
bool check_X(object x) { return extract<X>(x).check(); }
bool check_X_ptr(object x) { return extract<X*>(x).check(); }
bool check_X_ref(object x) { return extract<X&>(x).check(); }

std::string x_rep(X const& x)
{
    return "X("  + std::to_string(x.value()) + ")";
}

PXR_BOOST_PYTHON_MODULE(extract_ext)
{
    implicitly_convertible<int, X>();

    def("extract_bool", extract_bool);
    def("extract_list", extract_list);
    def("extract_cstring", extract_cstring);
    def("extract_string", extract_string);
    def("extract_string_cref", extract_string_cref, return_value_policy<reference_existing_object>());
    def("extract_X", extract_X);
    def("extract_X_ptr", extract_X_ptr, return_value_policy<reference_existing_object>());
    def("extract_X_ref", extract_X_ref, return_value_policy<reference_existing_object>());

    def("check_bool", check_bool);
    def("check_list", check_list);
    def("check_cstring", check_cstring);
    def("check_string", check_string);
    def("check_string_cref", check_string_cref);
    def("check_X", check_X);
    def("check_X_ptr", check_X_ptr);
    def("check_X_ref", check_X_ref);

    def("double_X", double_X);

    def("count_Xs", &X::count);
        ;

    object x_class(
        class_<X>("X", init<int>())
            .def( "__repr__", x_rep));
        
    // Instantiate an X object through the Python interface
    object x_obj = x_class(3);

    // Get the C++ object out of the Python object
    X const& x = extract<X&>(x_obj);
    if (x.value() != 3) {
        throw std::runtime_error("x.value() == 3 failure.");
    }
}


#include "module_tail.cpp"


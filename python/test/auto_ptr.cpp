//
// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// If BOOST_PYTHON_NO_PY_SIGNATURES was defined when building this module,
// boost::python will generate simplified docstrings that break the associated
// test unless we undefine it before including any headers.
#undef BOOST_PYTHON_NO_PY_SIGNATURES

#include "pxr/external/boost/python/module.hpp"
#include "test_class.hpp"
#include "pxr/external/boost/python/class.hpp"
#include "pxr/external/boost/python/docstring_options.hpp"
#include "pxr/external/boost/python/extract.hpp"
#include "pxr/external/boost/python/def.hpp"
#include "pxr/external/boost/python/implicit.hpp"

#include <boost/detail/workaround.hpp>

#include <memory>

using namespace boost::python;

typedef test_class<> X;

struct Y : X
{
    Y(int n) : X(n) {};
};

int look(std::auto_ptr<X> const& x)
{
    return (x.get()) ? x->value() : -1;
}

int steal(std::auto_ptr<X> x)
{
    return x->value();
}

int maybe_steal(std::auto_ptr<X>& x, bool doit)
{
    int n = x->value();
    if (doit)
        x.release();
    return n;
}

std::auto_ptr<X> make()
{
    return std::auto_ptr<X>(new X(77));
}

std::auto_ptr<X> callback(object f)
{
    std::auto_ptr<X> x(new X(77));
    return call<std::auto_ptr<X> >(f.ptr(), x);
}

std::auto_ptr<X> extract_(object o)
{
    return extract<std::auto_ptr<X>&>(o)
#if BOOST_MSVC <= 1300
        ()
#endif 
        ;
}

BOOST_PYTHON_MODULE(auto_ptr_ext)
{
    // Explicitly enable Python signatures in docstrings in case boost::python
    // was built with BOOST_PYTHON_NO_PY_SIGNATURES, which disables those
    // signatures by default.
    docstring_options doc_options;
    doc_options.enable_py_signatures();

    class_<X, std::auto_ptr<X>, boost::noncopyable>("X", init<int>())
        .def("value", &X::value)
        ;
    
    class_<Y, std::auto_ptr<Y>, bases<X>, boost::noncopyable>("Y", init<int>())
        ;

    // VC6 auto_ptrs do not have converting constructors    
#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, < 306)
    scope().attr("broken_auto_ptr") = 1;
#else
    scope().attr("broken_auto_ptr") = 0;
    implicitly_convertible<std::auto_ptr<Y>, std::auto_ptr<X> >();
#endif
    
    def("look", look);
    def("steal", steal);
    def("maybe_steal", maybe_steal);
    def("make", make);
    def("callback", callback);
    def("extract", extract_);
}

#include "module_tail.cpp"


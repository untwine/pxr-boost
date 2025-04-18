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

#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/return_value_policy.hpp>
#include <pxr/boost/python/manage_new_object.hpp>
#include <pxr/boost/python/reference_existing_object.hpp>
#include <pxr/boost/python/call_method.hpp>
#include <pxr/boost/python/pure_virtual.hpp>
#include <pxr/boost/python/def.hpp>

using namespace pxr::boost::python;

struct Callback
{
    Callback(PyObject* o) : mSelf(o) {}
    PyObject* mSelf;
};

struct P
{
    virtual ~P(){}
    virtual std::string f() = 0;
    std::string g() { return "P::g()"; }
};

struct PCallback : P, Callback
{
    PCallback (PyObject* self) : Callback(self) {}
    
    std::string f()
    {
        return call_method<std::string>(mSelf, "f");
    }
};

struct Q : virtual P
{
    std::string f() { return "Q::f()"; } 
};

struct A
{
    virtual ~A(){}
    virtual std::string f() { return "A::f()"; }
};

struct ACallback :  A,  Callback
{
    ACallback (PyObject* self) : Callback(self) {}
    
    
    std::string f()
    {
        return call_method<std::string>(mSelf, "f");
    }
    
    std::string default_f()
    {
        return A::f();
    }
};

struct B : A
{
    virtual std::string f() { return "B::f()"; } 
};

struct C : A
{
    virtual std::string f() { return "C::f()"; }
};

struct D : A
{
    virtual std::string f() { return "D::f()"; }
    std::string g() { return "D::g()"; }
};

struct DCallback :  D,  Callback
{
    DCallback (PyObject* self) : Callback(self) {}
     
    std::string f()
    {
        return call_method<std::string>(mSelf, "f");
    }
    
    std::string default_f()
    {
        return A::f();
    }
};

    
A& getBCppObj ()
{
    static B b;
    return b;
}

std::string call_f(A& a) { return a.f(); }

A* factory(unsigned choice)
{
    switch (choice % 3)
    {
    case 0: return new A;
        break;
    case 1: return new B;
        break;
    default: return new C;
        break;
    }
}

C& getCCppObj ()
{
    static C c;
    return c;
}

A* pass_a(A* x) { return x; }

PXR_BOOST_PYTHON_MODULE_INIT(polymorphism_ext)
{
    class_<A,noncopyable,ACallback>("A")
        .def("f", &A::f, &ACallback::default_f)
        ;
    
    def("getBCppObj", getBCppObj, return_value_policy<reference_existing_object>());

    class_<C,bases<A>,noncopyable>("C")
        .def("f", &C::f)
        ;
    
    class_<D,bases<A>,DCallback,noncopyable>("D")
        .def("f", &D::f, &DCallback::default_f)
        .def("g", &D::g)
        ;

    def("pass_a", &pass_a,  return_internal_reference<>());
    
    def("getCCppObj", getCCppObj, return_value_policy<reference_existing_object>());

    def("factory", factory, return_value_policy<manage_new_object>());

    def("call_f", call_f);

    class_<P,noncopyable,PCallback>("P")
        .def("f", pure_virtual(&P::f))
        ;

    class_<Q, bases<P> >("Q")
        .def("g", &P::g) // make sure virtual inheritance doesn't interfere
        ;
}

//#include "module_tail.cpp"

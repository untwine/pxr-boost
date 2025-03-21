// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python.hpp>
#include <memory>

using namespace pxr::boost;

class A : public std::enable_shared_from_this<A> {
 public:
   A() : val(0) {};
   int val;
   typedef std::shared_ptr<A> A_ptr;
   A_ptr self() {
      A_ptr self;
      self = shared_from_this();
      return self;
    }

};

class B {
  public:
    B() {
       a = A::A_ptr(new A());
    }
    void set(A::A_ptr _a) {
      this->a = _a;
    }
    A::A_ptr get() {
       return a;
    }
    A::A_ptr a;
};

template <class T>
void hold_python(std::shared_ptr<T>& x)
{
      x = python::extract<std::shared_ptr<T> >( python::object(x) );
}

A::A_ptr get_b_a(std::shared_ptr<B> b)
{
    hold_python(b->a);
    return b->get();
}

PXR_BOOST_PYTHON_MODULE(andreas_beyer_ext) {
  python::class_<A, python::noncopyable> ("A")
    .def("self", &A::self)
    .def_readwrite("val", &A::val)
  ;
  python::register_ptr_to_python< A::A_ptr >();
 
  python::class_<B>("B")
     .def("set", &B::set)
//     .def("get", &B::get)
     .def("get", get_b_a)
  ;
}

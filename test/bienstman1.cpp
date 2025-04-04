// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/reference_existing_object.hpp>
#include <pxr/boost/python/return_value_policy.hpp>

struct A {};

struct V 
{
 virtual ~V() {}; // silence compiler warningsa
 virtual void f() = 0;

 const A* inside() {return &a;}

 A a;
};

const A* outside(const V& v) {return &v.a;}

PXR_BOOST_PYTHON_MODULE(bienstman1_ext)
{
  using namespace pxr::boost::python;
  using pxr::boost::python::return_value_policy;
  using pxr::boost::python::reference_existing_object;

  class_<A>("A");

  class_<V, noncopyable>("V", no_init)
      .def("inside", &V::inside, 
           return_value_policy<reference_existing_object>())
      .def("outside", outside, 
           return_value_policy<reference_existing_object>())
      ;
}


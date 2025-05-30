// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//
//  example that shows problems with overloading and automatic conversion.
//  if you call one of the below functions from python with bool/int/double, 
//  you'll see that the overload called is first match, not best match.
//  See overload matching in luabind for an example of how to do this better.
//
//  see this mail:
//  http://mail.python.org/pipermail/cplusplus-sig/2009-March/014362.html
//
//  This test isn't called by the cmake/jamfiles.  For future use.
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/def.hpp>
#include <complex>
#include <pxr/boost/python/handle.hpp>
#include <pxr/boost/python/cast.hpp>
#include <pxr/boost/python/object.hpp>
#include <pxr/boost/python/detail/wrap_python.hpp>

using pxr::boost::python::def;
using pxr::boost::python::handle;
using pxr::boost::python::object;
using pxr::boost::python::borrowed;

std::string takes_bool(bool b) { return "bool"; }
std::string takes_int(int b) { return "int"; }
std::string takes_double(double b) { return "double"; }


PXR_BOOST_PYTHON_MODULE(overload_resolution)
{    
  def("bid", takes_bool);
  def("bid", takes_int);
  def("bid", takes_double);

  def("dib", takes_double);
  def("dib", takes_int);
  def("dib", takes_bool);

  def("idb", takes_int);
  def("idb", takes_double);
  def("idb", takes_bool);

  def("bdi", takes_bool);
  def("bdi", takes_double);
  def("bdi", takes_int);
}


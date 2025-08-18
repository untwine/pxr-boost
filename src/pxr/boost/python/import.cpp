// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright Stefan Seefeld 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include "pxr/boost/python/import.hpp"
#include "pxr/boost/python/borrowed.hpp"
#include "pxr/boost/python/extract.hpp"
#include "pxr/boost/python/handle.hpp"

namespace PXR_BOOST_NAMESPACE 
{ 
namespace python 
{ 

object PXR_BOOST_PYTHON_DECL import(str name)
{ 
  // should be 'char const *' but older python versions don't use 'const' yet.
  char *n = python::extract<char *>(name);
  python::handle<> module(PyImport_ImportModule(n));
  return python::object(module);
}

}  // namespace python
}  // namespace PXR_BOOST_NAMESPACE

// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright Ralf W. Grosse-Kunstleve 2002-2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

/*
    This example shows how to make an Extension Class "pickleable".

    The world class below can be fully restored by passing the
    appropriate argument to the constructor. Therefore it is sufficient
    to define the pickle interface method __getinitargs__.

    For more information refer to boost/libs/python/doc/pickle.html.
 */

#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/tuple.hpp>

#include <string>

namespace pxr_boost_python_test {

  // A friendly class.
  class world
  {
    private:
      std::string country;
    public:
      world(const std::string& _country) {
        this->country = _country;
      }
      std::string greet() const { return "Hello from " + country + "!"; }
      std::string get_country() const { return country; }
  };

  struct world_pickle_suite : pxr::boost::python::pickle_suite
  {
    static
    pxr::boost::python::tuple
    getinitargs(const world& w)
    {
        return pxr::boost::python::make_tuple(w.get_country());
    }
  };

  // To support test of "pickling not enabled" error message.
  struct noop {};
}

PXR_BOOST_PYTHON_MODULE(pickle1_ext)
{
  using namespace pxr::boost::python;
  using namespace pxr_boost_python_test;
  class_<world>("world", init<const std::string&>())
      .def("greet", &world::greet)
      .def_pickle(world_pickle_suite())
      ;

  // To support test of "pickling not enabled" error message.
  class_<noop>("noop");
}

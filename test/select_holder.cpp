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

#include <pxr/boost/python/object/class_metadata.hpp>
#include <pxr/boost/python/has_back_reference.hpp>
#include <pxr/boost/python/detail/not_specified.hpp>
#include <pxr/boost/python/detail/type_traits.hpp>
#include <pxr/boost/python/detail/mpl2/bool.hpp>
#include <memory>

struct BR {};

struct Base {};
struct Derived : Base {};

namespace pxr { namespace boost { namespace python
{
  // specialization
  template <>
  struct has_back_reference<BR>
    : detail::mpl2::true_
  {
  };
}}} // namespace pxr::boost::python

template <class T, class U>
void assert_same(U* = 0, T* = 0)
{
    static_assert((pxr::boost::python::detail::is_same<T,U>::value));
    
}

template <class T, class Held, class Holder>
void assert_holder(T* = 0, Held* = 0, Holder* = 0)
{
    using namespace pxr::boost::python::detail;
    using namespace pxr::boost::python::objects;
    
    typedef typename class_metadata<
       T,Held,not_specified,not_specified
           >::holder h;
    
    assert_same<Holder>(
        (h*)0
    );
}

int test_main(int, char * [])
{
    using namespace pxr::boost::python::detail;
    using namespace pxr::boost::python::objects;

    assert_holder<Base,not_specified,value_holder<Base> >();

    assert_holder<BR,not_specified,value_holder_back_reference<BR,BR> >();
    assert_holder<Base,Base,value_holder_back_reference<Base,Base> >();
    assert_holder<BR,BR,value_holder_back_reference<BR,BR> >();

    assert_holder<Base,Derived
        ,value_holder_back_reference<Base,Derived> >();

    return 0;
}


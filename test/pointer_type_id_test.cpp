// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python/type_id.hpp>

#include <cassert>
#include <pxr/boost/python/converter/pointer_type_id.hpp>

int main()
{
    using namespace pxr::boost::python::converter;
    
    pxr::boost::python::type_info x
        = pxr::boost::python::type_id<int>();
    

    assert(pointer_type_id<int*>() == x);
    assert(pointer_type_id<int const*>() == x);
    assert(pointer_type_id<int volatile*>() == x);
    assert(pointer_type_id<int const volatile*>() == x);
    
    assert(pointer_type_id<int*&>() == x);
    assert(pointer_type_id<int const*&>() == x);
    assert(pointer_type_id<int volatile*&>() == x);
    assert(pointer_type_id<int const volatile*&>() == x);
    
    assert(pointer_type_id<int*const&>() == x);
    assert(pointer_type_id<int const*const&>() == x);
    assert(pointer_type_id<int volatile*const&>() == x);
    assert(pointer_type_id<int const volatile*const&>() == x);
    
    assert(pointer_type_id<int*volatile&>() == x);
    assert(pointer_type_id<int const*volatile&>() == x);
    assert(pointer_type_id<int volatile*volatile&>() == x);
    assert(pointer_type_id<int const volatile*volatile&>() == x);
    
    assert(pointer_type_id<int*const volatile&>() == x);
    assert(pointer_type_id<int const*const volatile&>() == x);
    assert(pointer_type_id<int volatile*const volatile&>() == x);
    assert(pointer_type_id<int const volatile*const volatile&>() == x);
    
    return 0;
}

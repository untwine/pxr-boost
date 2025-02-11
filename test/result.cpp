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

#include <pxr/boost/python/detail/result.hpp>
#include <pxr/boost/python/type.hpp>
#include <functional>

using pxr::boost::python::detail::result;
using pxr::boost::python::type;

void expect_int(type<int>*) {}
void expect_string(type<char*>*) {}

struct X {};

int main()
{
    // Test the usage which works for functions, member functions, and data members
    expect_int(
        result((int(*)())0)
        );

    expect_int(
        result((int(*)(char))0)
        );

    expect_int(
        result((int(X::*)())0)
        );

    expect_int(
        result((int(X::*)(char))0)
        );

    expect_int(
        result((int(X::*))0)
        );

    expect_string(
        result((char*(*)())0)
        );
    
    expect_string(
        result((char*(*)(char))0)
        );
    
    expect_string(
        result((char*(X::*)())0)
        );
    
    expect_string(
        result((char*(X::*)(char))0)
        );
    
    expect_string(
        result((char*(X::*))0)
        );
    
    // Show that we can use the general version that works for
    // AdaptableFunctions
    expect_int(
        result((int(*)())0,0)
        );

    expect_int(
        result((int(*)(char))0,0)
        );

    expect_int(
        result((int(X::*)())0,0)
        );

    expect_int(
        result((int(X::*)(char))0,0)
        );

    expect_int(
        result((int(X::*))0,0)
        );
    
    expect_int(
        result(std::plus<int>(),0)
        );

    expect_string(
        result((char*(*)())0,0)
        );
    
    expect_string(
        result((char*(*)(char))0,0)
        );
    
    expect_string(
        result((char*(X::*)())0,0)
        );
    
    expect_string(
        result((char*(X::*)(char))0,0)
        );
    
    expect_string(
        result((char*(X::*))0,0)
        );
    
    expect_string(
        result(std::plus<char*>(),0)
        );

    return 0;
}

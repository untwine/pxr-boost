//
// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include "../bases.hpp"
#include "../detail/type_traits.hpp"

struct A;
struct B;

template <class X, class Y, class Z>
struct choose_bases
    : pxr::boost::python::detail::select_bases<
    X
    , typename pxr::boost::python::detail::select_bases<
        Y
        , typename pxr::boost::python::detail::select_bases<Z>::type
    >::type>
{
    
};

int main()
{
    static_assert((pxr::boost::python::detail::specifies_bases<
                         pxr::boost::python::bases<A,B> >::value));

    static_assert((!pxr::boost::python::detail::specifies_bases<
                         pxr::boost::python::bases<A,B>& >::value));

    static_assert((!pxr::boost::python::detail::specifies_bases<
                         void* >::value));

    static_assert((!pxr::boost::python::detail::specifies_bases<
                         int >::value));

    static_assert((!pxr::boost::python::detail::specifies_bases<
                         int[5] >::value));

    typedef pxr::boost::python::detail::select_bases<
        int
        , pxr::boost::python::detail::select_bases<char*>::type > collected1;

    static_assert((pxr::boost::python::detail::is_same<collected1::type,pxr::boost::python::bases<> >::value));
    static_assert((pxr::boost::python::detail::is_same<choose_bases<int,char*,long>::type,pxr::boost::python::bases<> >::value));
    
    typedef pxr::boost::python::detail::select_bases<
        int
        , pxr::boost::python::detail::select_bases<
                pxr::boost::python::bases<A,B>
                , pxr::boost::python::detail::select_bases<
                        A
            >::type
         >::type
     > collected2;

    static_assert((pxr::boost::python::detail::is_same<collected2::type,pxr::boost::python::bases<A,B> >::value));
    static_assert((pxr::boost::python::detail::is_same<choose_bases<int,pxr::boost::python::bases<A,B>,long>::type,pxr::boost::python::bases<A,B> >::value));
    
    return 0;
}

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
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/return_internal_reference.hpp>
#include <pxr/boost/python/copy_non_const_reference.hpp>
#include <pxr/boost/python/return_value_policy.hpp>
#include <pxr/boost/python/iterator.hpp>
#include <list>
#include <utility>
#include <iterator>
#include <algorithm>

using namespace pxr::boost::python;

typedef std::list<int> list_int;
typedef std::list<list_int> list_list;


void push_back(list_int& x, int y)
{
    x.push_back(y);
}

void push_list_back(list_list& x, list_int const& y)
{
    x.push_back(y);
}

int back(list_int& x)
{
    return x.back();
}

typedef std::pair<list_int::iterator,list_int::iterator> list_range;

struct list_range2 : list_range
{
    list_int::iterator& begin() { return this->first; }
    list_int::iterator& end() { return this->second; }
};

list_range range(list_int& x)
{
    return list_range(x.begin(), x.end());
}

struct two_lists
{
    two_lists()
    {
        int primes[] = { 2, 3, 5, 7, 11, 13 };
        std::copy(primes, primes + sizeof(primes)/sizeof(*primes), std::back_inserter(one));
        int evens[] = { 2, 4, 6, 8, 10, 12 };
        std::copy(evens, evens + sizeof(evens)/sizeof(*evens), std::back_inserter(two));
    }

    struct two_start
    {
        typedef list_int::iterator result_type;
        result_type operator()(two_lists& ll) const { return ll.two.begin(); }
    };
    friend struct two_start;
    
    list_int::iterator one_begin() { return one.begin(); }
    list_int::iterator two_begin() { return two.begin(); }

    list_int::iterator one_end() { return one.end(); }
    list_int::iterator two_end() { return two.end(); }
        
private:
    list_int one;
    list_int two;
};

PXR_BOOST_PYTHON_MODULE(iterator_ext)
{
    using pxr::boost::python::iterator; // gcc 2.96 bug workaround
    def("range", &::range);

    class_<list_int>("list_int")
        .def("push_back", push_back)
        .def("back", back)
        .def("__iter__", iterator<list_int>())
        ;

    class_<list_range>("list_range")

        // We can specify data members
        .def("__iter__"
             , range(&list_range::first, &list_range::second))
        ;

    // No runtime tests for this one yet
    class_<list_range2>("list_range2")

        // We can specify member functions returning a non-const reference
        .def("__iter__", range(&list_range2::begin, &list_range2::end))
        ;

    class_<two_lists>("two_lists")

        // We can spcify member functions
        .add_property(
            "primes"
            , range(&two_lists::one_begin, &two_lists::one_end))

        // Prove that we can explicitly specify call policies
        .add_property(
            "evens"
            , range<return_value_policy<copy_non_const_reference> >(
                &two_lists::two_begin, &two_lists::two_end))

        // Prove that we can specify call policies and target
        .add_property(
            "twosies"
            , range<return_value_policy<copy_non_const_reference>, two_lists>(
                two_lists::two_start()
                , &two_lists::two_end))
        ;

    class_<list_list>("list_list")
        .def("push_back", push_list_back)
        .def("__iter__", iterator<list_list,return_internal_reference<> >())
        ;
}

#include "module_tail.cpp"

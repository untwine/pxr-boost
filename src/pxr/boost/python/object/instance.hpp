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

#ifndef PXR_BOOST_PYTHON_OBJECT_INSTANCE_HPP
# define PXR_BOOST_PYTHON_OBJECT_INSTANCE_HPP

# include "../detail/prefix.hpp"
# include "../detail/type_traits.hpp"
# include <cstddef>

namespace pxr { namespace boost { namespace python
{
  struct instance_holder;
}}} // namespace pxr::boost::python

namespace pxr { namespace boost { namespace python { namespace objects {

// Each extension instance will be one of these
template <class Data = char>
struct instance
{
    PyObject_VAR_HEAD
    PyObject* dict;
    PyObject* weakrefs; 
    instance_holder* objects;

    struct
    {
        alignas(Data) char bytes[sizeof(Data)];
    } storage;
};

template <class Data>
struct additional_instance_size
{
    typedef instance<Data> instance_data;
    typedef instance<char> instance_char;
    static constexpr std::size_t
                          value = sizeof(instance_data) -
                             PXR_BOOST_PYTHON_OFFSETOF(instance_char,storage) +
                             pxr::boost::python::detail::alignment_of<Data>::value;
};

}}}} // namespace pxr::boost::python::object

#endif // PXR_BOOST_PYTHON_OBJECT_INSTANCE_HPP

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

#ifndef PXR_BOOST_PYTHON_OBJECT_ITEMS_HPP
# define PXR_BOOST_PYTHON_OBJECT_ITEMS_HPP

# include "./detail/prefix.hpp"

# include "./proxy.hpp"
# include "./object_core.hpp"
# include "./object_protocol.hpp"

namespace pxr { namespace boost { namespace python { namespace api {

struct const_item_policies
{
    typedef object key_type;
    static object get(object const& target, object const& key);
};
  
struct item_policies : const_item_policies
{
    static object const& set(object const& target, object const& key, object const& value);
    static void del(object const& target, object const& key);
};

//
// implementation
//
template <class U>
inline object_item
object_operators<U>::operator[](object_cref key)
{
    object_cref2 x = *static_cast<U*>(this);
    return object_item(x, key);
}

template <class U>
inline const_object_item
object_operators<U>::operator[](object_cref key) const
{
    object_cref2 x = *static_cast<U const*>(this);
    return const_object_item(x, key);
}

template <class U>
template <class T>
inline const_object_item
object_operators<U>::operator[](T const& key) const
{
    return (*this)[object(key)];
}

template <class U>
template <class T>
inline object_item
object_operators<U>::operator[](T const& key)
{
    return (*this)[object(key)];
}

inline object const_item_policies::get(object const& target, object const& key)
{
    return getitem(target, key);
}

inline object const& item_policies::set(
    object const& target
    , object const& key
    , object const& value)
{
    setitem(target, key, value);
    return value;
}

inline void item_policies::del(
    object const& target
    , object const& key)
{
    delitem(target, key);
}

}}}} // namespace pxr::boost::python::api

#endif // PXR_BOOST_PYTHON_OBJECT_ITEMS_HPP

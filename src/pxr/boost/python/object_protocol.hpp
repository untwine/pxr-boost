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

#ifndef PXR_BOOST_PYTHON_OBJECT_PROTOCOL_HPP
# define PXR_BOOST_PYTHON_OBJECT_PROTOCOL_HPP

# include "./detail/prefix.hpp"

# include "./object_protocol_core.hpp"
# include "./object_core.hpp"


namespace pxr { namespace boost { namespace python { namespace api {

template <class Target, class Key>
object getattr(Target const& target, Key const& key)
{
    return getattr(object(target), object(key));
}

template <class Target, class Key, class Default>
object getattr(Target const& target, Key const& key, Default const& default_)
{
    return getattr(object(target), object(key), object(default_));
}


template <class Key, class Value>
void setattr(object const& target, Key const& key, Value const& value)
{
    setattr(target, object(key), object(value));
}

template <class Key>
void delattr(object const& target, Key const& key)
{
    delattr(target, object(key));
}

template <class Target, class Key>
object getitem(Target const& target, Key const& key)
{
    return getitem(object(target), object(key));
}


template <class Key, class Value>
void setitem(object const& target, Key const& key, Value const& value)
{
    setitem(target, object(key), object(value));
}

template <class Key>
void delitem(object const& target, Key const& key)
{
    delitem(target, object(key));
}

template <class Target, class Begin, class End>
object getslice(Target const& target, Begin const& begin, End const& end)
{
    return getslice(object(target), object(begin), object(end));
}

template <class Begin, class End, class Value>
void setslice(object const& target, Begin const& begin, End const& end, Value const& value)
{
    setslice(target, object(begin), object(end), object(value));
}

template <class Begin, class End>
void delslice(object const& target, Begin const& begin, End const& end)
{
    delslice(target, object(begin), object(end));
}

}}}} // namespace pxr::boost::python::api

#endif // PXR_BOOST_PYTHON_OBJECT_PROTOCOL_HPP

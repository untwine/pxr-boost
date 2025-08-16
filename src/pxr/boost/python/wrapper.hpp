// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_WRAPPER_HPP
# define PXR_BOOST_PYTHON_WRAPPER_HPP

#include "pxr/boost/python/common.hpp"

# include "pxr/boost/python/detail/wrapper_base.hpp"
# include "pxr/boost/python/override.hpp"
# include "pxr/boost/python/converter/registered.hpp"
# include "pxr/boost/python/detail/sfinae.hpp"

namespace PXR_BOOST_NAMESPACE { namespace python { 

template <class T>
class wrapper : public detail::wrapper_base
{
 public:
    // Do not touch this implementation detail!
    typedef T _wrapper_wrapped_type_;

 protected:
    override get_override(char const* name) const
    {
        typedef detail::wrapper_base base;
        converter::registration const& r
            = converter::registered<T>::converters;
        PyTypeObject* type = r.get_class_object();
        
        return this->base::get_override(name, type);
    }
};

}} // namespace PXR_BOOST_NAMESPACE::python

#endif // PXR_BOOST_PYTHON_WRAPPER_HPP

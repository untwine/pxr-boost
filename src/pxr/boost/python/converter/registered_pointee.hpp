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

#ifndef PXR_BOOST_PYTHON_CONVERTER_REGISTERED_POINTEE_HPP
# define PXR_BOOST_PYTHON_CONVERTER_REGISTERED_POINTEE_HPP

# include "./registered.hpp"
# include "./pointer_type_id.hpp"
# include "./registry.hpp"
# include "../detail/type_traits.hpp"

namespace pxr { namespace boost { namespace python { namespace converter {

struct registration;

template <class T>
struct registered_pointee
    : registered<
        typename pxr::boost::python::detail::remove_pointer<
           typename pxr::boost::python::detail::remove_cv<
              typename pxr::boost::python::detail::remove_reference<T>::type
           >::type
        >::type
    >
{
};
}}}} // namespace pxr::boost::python::converter

#endif // PXR_BOOST_PYTHON_CONVERTER_REGISTERED_POINTEE_HPP

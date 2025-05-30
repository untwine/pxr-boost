// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_REF_HPP
#define PXR_BOOST_PYTHON_REF_HPP

#include <functional>

namespace pxr { namespace boost { namespace python {

using std::ref;
using std::cref;
using std::reference_wrapper;

template <class T>
struct is_reference_wrapper : std::false_type
{
};

template <class T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type
{
};

template <class T>
struct unwrap_reference
{
    using type = T;
};

template <class U>
struct unwrap_reference<std::reference_wrapper<U>>
{
    using type = U;
};

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_REF_HPP

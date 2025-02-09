// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_TYPE_LIST_HPP
#define PXR_BOOST_PYTHON_TYPE_LIST_HPP

#ifndef PXR_USE_INTERNAL_BOOST_PYTHON

#include <boost/mpl/vector.hpp>

namespace pxr { namespace boost { namespace python {

template <class ...T>
using type_list = ::boost::mpl::vector<T...>;

}}} // namespace pxr::boost::python

#else

#include "./detail/type_list.hpp"

namespace pxr { namespace boost { namespace python {

template <class ...T>
using type_list = detail::type_list<T...>;

}}} // namespace pxr::boost::python

#endif // PXR_USE_INTERNAL_BOOST_PYTHON

#endif // PXR_BOOST_PYTHON_TYPE_LIST_HPP

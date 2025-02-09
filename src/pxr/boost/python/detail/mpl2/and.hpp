// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_DETAIL_MPL2_AND_HPP
#define PXR_BOOST_PYTHON_DETAIL_MPL2_AND_HPP

#include <type_traits>

namespace pxr { namespace boost { namespace python { namespace detail { namespace mpl2 {

template <typename ...F>
using and_ = std::conjunction<F...>;

}}}}} // namespace pxr::boost::python::detail::mpl2

#endif

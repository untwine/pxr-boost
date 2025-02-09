// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_NONCOPYABLE_HPP
#define PXR_BOOST_PYTHON_NONCOPYABLE_HPP

#ifndef PXR_USE_INTERNAL_BOOST_PYTHON

#include <boost/noncopyable.hpp>

namespace pxr { namespace boost { namespace python {

using boost::noncopyable;

}}} // namespace pxr::boost::python

#else

namespace pxr { namespace boost { namespace python {

// Tag structure used with class_ to indicate that the wrapped C++ type
// is not copyable. Note this struct is marked final to avoid client
// code accidentally inheriting from it instead of boost::noncopyable.
struct noncopyable final
{
};

}}} // namespace pxr::boost::python

#endif // PXR_USE_INTERNAL_BOOST_PYTHON

#endif // PXR_BOOST_PYTHON_NONCOPYABLE_HPP

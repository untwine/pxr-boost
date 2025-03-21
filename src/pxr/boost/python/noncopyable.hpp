// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_NONCOPYABLE_HPP
#define PXR_BOOST_PYTHON_NONCOPYABLE_HPP

namespace pxr { namespace boost { namespace python {

// Tag structure used with class_ to indicate that the wrapped C++ type
// is not copyable. Note this struct is marked final to avoid client
// code accidentally inheriting from it instead of boost::noncopyable.
struct noncopyable final
{
};

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_NONCOPYABLE_HPP

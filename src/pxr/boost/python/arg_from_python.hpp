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

#ifndef PXR_BOOST_PYTHON_ARG_FROM_PYTHON_HPP
# define PXR_BOOST_PYTHON_ARG_FROM_PYTHON_HPP

# include "./detail/prefix.hpp"
# include "./converter/arg_from_python.hpp"

namespace pxr { namespace boost { namespace python {

template <class T>
struct arg_from_python
    : converter::select_arg_from_python<
          T
      >::type
{
    typedef typename converter::select_arg_from_python<
          T
        >::type base;
    
    arg_from_python(PyObject*);
};

// specialization for PyObject*
template <>
struct arg_from_python<PyObject*>
{
    typedef PyObject* result_type;
    
    arg_from_python(PyObject* p) : m_source(p) {}
    bool convertible() const { return true; }
    PyObject* operator()() const { return m_source; }
 private:
    PyObject* m_source;
};

template <>
struct arg_from_python<PyObject* const&>
{
    typedef PyObject* const& result_type;
    
    arg_from_python(PyObject* p) : m_source(p) {}
    bool convertible() const { return true; }
    PyObject*const& operator()() const { return m_source; }
 private:
    PyObject* m_source;
};

//
// implementations
//
template <class T>
inline arg_from_python<T>::arg_from_python(PyObject* source)
    : base(source)
{
}

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_ARG_FROM_PYTHON_HPP

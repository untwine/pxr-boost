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

#ifndef PXR_BOOST_PYTHON_SCOPE_HPP
# define PXR_BOOST_PYTHON_SCOPE_HPP

# include "./detail/prefix.hpp"
# include "./object.hpp"
# include "./refcount.hpp"

namespace pxr { namespace boost { namespace python {

namespace detail
{
  // Making this a namespace-scope variable to avoid Cygwin issues.
  // Use a PyObject* to avoid problems with static destruction after Py_Finalize
  extern PXR_BOOST_PYTHON_DECL PyObject* current_scope;
}

class scope
  : public object
{
 public:
    inline scope(scope const&);
    inline scope(object const&);
    inline scope();
    inline ~scope();
    
 private: // data members
    PyObject* m_previous_scope;

 private: // unimplemented functions
    void operator=(scope const&);
};

inline scope::scope(object const& new_scope)
    : object(new_scope)
    , m_previous_scope(detail::current_scope)
{
    detail::current_scope = python::incref(new_scope.ptr());
}

inline scope::scope()
    : object(detail::borrowed_reference(
                 detail::current_scope ? detail::current_scope : Py_None
                 ))
    , m_previous_scope(python::xincref(detail::current_scope))
{
}

inline scope::~scope()
{
    python::xdecref(detail::current_scope);
    detail::current_scope = m_previous_scope;
}

namespace converter
{
  template <>
  struct object_manager_traits<scope>
      : object_manager_traits<object>
  {
  };
}

// Placing this after the specialization above suppresses a CWPro8.3 bug
inline scope::scope(scope const& new_scope)
    : object(new_scope)
    , m_previous_scope(detail::current_scope)
{
    detail::current_scope = python::incref(new_scope.ptr());
}

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_SCOPE_HPP

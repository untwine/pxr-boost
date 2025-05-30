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

#ifndef PXR_BOOST_PYTHON_OBJECT_PY_FUNCTION_HPP
# define PXR_BOOST_PYTHON_OBJECT_PY_FUNCTION_HPP

# include "../detail/signature.hpp"
# include "../detail/mpl2/size.hpp"
# include <memory>

namespace pxr { namespace boost { namespace python { namespace objects {

// This type is used as a "generalized Python callback", wrapping the
// function signature:
//
//      PyObject* (PyObject* args, PyObject* keywords)

struct PXR_BOOST_PYTHON_DECL py_function_impl_base
{
    virtual ~py_function_impl_base();
    virtual PyObject* operator()(PyObject*, PyObject*) = 0;
    virtual unsigned min_arity() const = 0;
    virtual unsigned max_arity() const;
    virtual python::detail::py_func_sig_info signature() const = 0;
};

template <class Caller>
struct caller_py_function_impl : py_function_impl_base
{
    caller_py_function_impl(Caller const& caller)
        : m_caller(caller)
    {}
    
    PyObject* operator()(PyObject* args, PyObject* kw)
    {
        return m_caller(args, kw);
    }
    
    virtual unsigned min_arity() const
    {
        return m_caller.min_arity();
    }
    
    virtual python::detail::py_func_sig_info signature() const
    {
        return m_caller.signature();
    }

 private:
    Caller m_caller;
};

template <class Caller, class Sig>
struct signature_py_function_impl : py_function_impl_base
{
    signature_py_function_impl(Caller const& caller)
        : m_caller(caller)
    {}
    
    PyObject* operator()(PyObject* args, PyObject* kw)
    {
        return m_caller(args, kw);
    }
    
    virtual unsigned min_arity() const
    {
        return python::detail::mpl2::size<Sig>::value - 1;
    }
    
    virtual python::detail::py_func_sig_info signature() const
    {
        python::detail::signature_element const* sig = python::detail::signature<Sig>::elements();
        python::detail::py_func_sig_info res = {sig, sig};
        return  res;
    }

 private:
    Caller m_caller;
};

template <class Caller, class Sig>
struct full_py_function_impl : py_function_impl_base
{
    full_py_function_impl(Caller const& caller, unsigned min_arity, unsigned max_arity)
      : m_caller(caller)
      , m_min_arity(min_arity)
      , m_max_arity(max_arity > min_arity ? max_arity : min_arity)
    {}
    
    PyObject* operator()(PyObject* args, PyObject* kw)
    {
        return m_caller(args, kw);
    }
    
    virtual unsigned min_arity() const
    {
        return m_min_arity;
    }
    
    virtual unsigned max_arity() const
    {
        return m_max_arity;
    }
    
    virtual python::detail::py_func_sig_info signature() const
    {
        python::detail::signature_element const* sig = python::detail::signature<Sig>::elements();
        python::detail::py_func_sig_info res = {sig, sig};
        return  res;
    }

 private:
    Caller m_caller;
    unsigned m_min_arity;
    unsigned m_max_arity;
};

struct py_function
{
    template <class Caller>
    py_function(Caller const& caller)
        : m_impl(new caller_py_function_impl<Caller>(caller))
    {}

    template <class Caller, class Sig>
    py_function(Caller const& caller, Sig)
      : m_impl(new signature_py_function_impl<Caller, Sig>(caller))
    {}

    template <class Caller, class Sig>
    py_function(Caller const& caller, Sig, int min_arity, int max_arity = 0)
      : m_impl(new full_py_function_impl<Caller, Sig>(caller, min_arity, max_arity))
    {}

    py_function(py_function const& rhs)
      : m_impl(std::move(rhs.m_impl))
    {}

    PyObject* operator()(PyObject* args, PyObject* kw) const
    {
        return (*m_impl)(args, kw);
    }

    unsigned min_arity() const
    {
        return m_impl->min_arity();
    }
    
    unsigned max_arity() const
    {
        return m_impl->max_arity();
    }

    python::detail::signature_element const* signature() const
    {
        return m_impl->signature().signature;
    }

    python::detail::signature_element const& get_return_type() const
    {
        return *m_impl->signature().ret;
    }
    
 private:
    mutable std::unique_ptr<py_function_impl_base> m_impl;
};

}}}} // namespace pxr::boost::python::objects

#endif // PXR_BOOST_PYTHON_OBJECT_PY_FUNCTION_HPP

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

#ifndef PXR_BOOST_PYTHON_PROXY_HPP
# define PXR_BOOST_PYTHON_PROXY_HPP

# include "./detail/prefix.hpp"
# include "./object_core.hpp"
# include "./object_operators.hpp"

namespace pxr { namespace boost { namespace python { namespace api {

template <class Policies>
class proxy : public object_operators<proxy<Policies> >
{
    typedef typename Policies::key_type key_type;
    
    typedef proxy const& assignment_self;
 public:
    proxy(object const& target, key_type const& key);
    operator object() const;

    // to support a[b] = c[d]
    proxy const& operator=(assignment_self) const;
    
    template <class T>
    inline proxy const& operator=(T const& rhs) const
    {
        Policies::set(m_target, m_key, object(rhs));
        return *this;
    }

 public: // implementation detail
    void del() const;
        
 private:
    object m_target;
    key_type m_key;
};


template <class T>
inline void del(proxy<T> const& x)
{
    x.del();
}

//
// implementation
//

template <class Policies>
inline proxy<Policies>::proxy(object const& target, key_type const& key)
    : m_target(target), m_key(key)
{}

template <class Policies>
inline proxy<Policies>::operator object() const
{
    return Policies::get(m_target, m_key);
}

// to support a[b] = c[d]
template <class Policies>
inline proxy<Policies> const& proxy<Policies>::operator=(typename proxy::assignment_self rhs) const
{
    return *this = python::object(rhs);
}

# define PXR_BOOST_PYTHON_PROXY_INPLACE(op)                                         \
template <class Policies, class R>                                              \
proxy<Policies> const& operator op(proxy<Policies> const& lhs, R const& rhs)    \
{                                                                               \
    object old(lhs);                                                            \
    return lhs = (old op rhs);                                                  \
} 
PXR_BOOST_PYTHON_PROXY_INPLACE(+=)
PXR_BOOST_PYTHON_PROXY_INPLACE(-=)
PXR_BOOST_PYTHON_PROXY_INPLACE(*=)
PXR_BOOST_PYTHON_PROXY_INPLACE(/=)
PXR_BOOST_PYTHON_PROXY_INPLACE(%=)
PXR_BOOST_PYTHON_PROXY_INPLACE(<<=)
PXR_BOOST_PYTHON_PROXY_INPLACE(>>=)
PXR_BOOST_PYTHON_PROXY_INPLACE(&=)
PXR_BOOST_PYTHON_PROXY_INPLACE(^=)
PXR_BOOST_PYTHON_PROXY_INPLACE(|=)
# undef PXR_BOOST_PYTHON_PROXY_INPLACE

template <class Policies>
inline void proxy<Policies>::del() const
{
    Policies::del(m_target, m_key);
}

}}}} // namespace pxr::boost::python::api

#endif // PXR_BOOST_PYTHON_PROXY_HPP

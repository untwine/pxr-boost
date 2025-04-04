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

#ifndef PXR_BOOST_PYTHON_OBJECT_MAKE_PTR_INSTANCE_HPP
# define PXR_BOOST_PYTHON_OBJECT_MAKE_PTR_INSTANCE_HPP

# include "./make_instance.hpp"
# include "../converter/registry.hpp"
# include "../detail/type_traits.hpp"
# include "../detail/get_pointer.hpp"
# include <typeinfo>

namespace pxr { namespace boost { namespace python { namespace objects {

template <class T, class Holder>
struct make_ptr_instance
    : make_instance_impl<T, Holder, make_ptr_instance<T,Holder> >
{
    template <class Arg>
    static inline Holder* construct(void* storage, PyObject*, Arg& x)
    {
      return new (storage) Holder(std::move(x));
    }
    
    template <class Ptr>
    static inline PyTypeObject* get_class_object(Ptr const& x)
    {
        using python::detail::get_pointer;
        return get_class_object_impl(get_pointer(x));
    }
#ifndef PXR_BOOST_PYTHON_NO_PY_SIGNATURES
    static inline PyTypeObject const* get_pytype()
    {
        return converter::registered<T>::converters.get_class_object();
    }
#endif
 private:
    template <class U>
    static inline PyTypeObject* get_class_object_impl(U const volatile* p)
    {
        if (p == 0)
            return 0; // means "return None".

        PyTypeObject* derived = get_derived_class_object(
            typename pxr::boost::python::detail::is_polymorphic<U>::type(), p);
        
        if (derived)
            return derived;
        return converter::registered<T>::converters.get_class_object();
    }
    
    template <class U>
    static inline PyTypeObject* get_derived_class_object(pxr::boost::python::detail::true_, U const volatile* x)
    {
        converter::registration const* r = converter::registry::query(
            type_info(typeid(*x))
        );
        return r ? r->m_class_object : 0;
    }
    
    template <class U>
    static inline PyTypeObject* get_derived_class_object(pxr::boost::python::detail::false_, U*)
    {
        return 0;
    }
};
  

}}}} // namespace pxr::boost::python::object

#endif // PXR_BOOST_PYTHON_OBJECT_MAKE_PTR_INSTANCE_HPP

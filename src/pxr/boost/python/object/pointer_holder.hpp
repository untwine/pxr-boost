// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

# ifndef PXR_BOOST_PYTHON_OBJECT_POINTER_HOLDER_HPP
#  define PXR_BOOST_PYTHON_OBJECT_POINTER_HOLDER_HPP

#  include "../type.hpp"

#  include "../instance_holder.hpp"
#  include "./inheritance_query.hpp"
#  include "./forward.hpp"

#  include "../pointee.hpp"
#  include "../type_id.hpp"

#  include "../detail/wrapper_base.hpp"
#  include "../detail/force_instantiate.hpp"
#  include "../detail/preprocessor.hpp"
#  include "../detail/type_traits.hpp"
#  include "../detail/get_pointer.hpp"

namespace pxr { namespace boost { namespace python {

template <class T> class wrapper;

}}}


namespace pxr { namespace boost { namespace python { namespace objects {

template <class Pointer, class Value>
struct pointer_holder : instance_holder
{
    typedef Value value_type;
    
    pointer_holder(Pointer);

    // Forward construction to the held object

    template<class... A>
    pointer_holder(PyObject* self, A... a)
        : m_p(new Value(objects::do_unforward(a,0)...))
    {
        using python::detail::get_pointer;
        python::detail::initialize_wrapper(self, get_pointer(this->m_p));
    }

 private: // types
    
 private: // required holder implementation
    void* holds(type_info, bool null_ptr_only);
    
    template <class T>
    inline void* holds_wrapped(type_info dst_t, wrapper<T>*,T* p)
    {
        return python::type_id<T>() == dst_t ? p : 0;
    }
    
    inline void* holds_wrapped(type_info, ...)
    {
        return 0;
    }

 private: // data members
    Pointer m_p;
};

template <class Pointer, class Value>
struct pointer_holder_back_reference : instance_holder
{
 private:
    typedef typename python::pointee<Pointer>::type held_type;
 public:
    typedef Value value_type;

    // Not sure about this one -- can it work? The source object
    // undoubtedly does not carry the correct back reference pointer.
    pointer_holder_back_reference(Pointer);

    // Forward construction to the held object
    template <class... A>
    pointer_holder_back_reference(
        PyObject* p, A... a)
        : m_p(new held_type(p, objects::do_unforward(a, 0)...))
    {}

 private: // required holder implementation
    void* holds(type_info, bool null_ptr_only);

 private: // data members
    Pointer m_p;
};

template <class Pointer, class Value>
inline pointer_holder<Pointer,Value>::pointer_holder(Pointer p)
    : m_p(std::move(p))
{
}

template <class Pointer, class Value>
inline pointer_holder_back_reference<Pointer,Value>::pointer_holder_back_reference(Pointer p)
    : m_p(std::move(p))
{
}

template <class Pointer, class Value>
void* pointer_holder<Pointer, Value>::holds(type_info dst_t, bool null_ptr_only)
{
    typedef typename pxr::boost::python::detail::remove_const< Value >::type non_const_value;
    using python::detail::get_pointer;

    if (dst_t == python::type_id<Pointer>()
        && !(null_ptr_only && get_pointer(this->m_p))
    )
        return &this->m_p;

    Value* p0
        = get_pointer(this->m_p)
        ;
    non_const_value* p = const_cast<non_const_value*>( p0 );

    if (p == 0)
        return 0;
    
    if (void* wrapped = holds_wrapped(dst_t, p, p))
        return wrapped;
    
    type_info src_t = python::type_id<non_const_value>();
    return src_t == dst_t ? p : find_dynamic_type(p, src_t, dst_t);
}

template <class Pointer, class Value>
void* pointer_holder_back_reference<Pointer, Value>::holds(type_info dst_t, bool null_ptr_only)
{
    using python::detail::get_pointer;

    if (dst_t == python::type_id<Pointer>()
        && !(null_ptr_only && get_pointer(this->m_p))
    )
        return &this->m_p;

    if (!get_pointer(this->m_p))
        return 0;
    
    Value* p = get_pointer(m_p);
    
    if (dst_t == python::type_id<held_type>())
        return p;

    type_info src_t = python::type_id<Value>();
    return src_t == dst_t ? p : find_dynamic_type(p, src_t, dst_t);
}

}}}} // namespace pxr::boost::python::objects

# endif // PXR_BOOST_PYTHON_OBJECT_POINTER_HOLDER_HPP

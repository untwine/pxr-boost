// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright Gottfried Ganßauge 2003..2006.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

/*
 * Generic Conversion of opaque C++-pointers to a Python-Wrapper.
 */
# ifndef PXR_BOOST_PYTHON_OPAQUE_POINTER_CONVERTER_HPP
# define PXR_BOOST_PYTHON_OPAQUE_POINTER_CONVERTER_HPP

# include "./detail/prefix.hpp"
# include "./lvalue_from_pytype.hpp"
# include "./to_python_converter.hpp"
# include "./converter/registrations.hpp"
# include "./detail/dealloc.hpp"
# include "./detail/type_traits.hpp"
# include "./detail/none.hpp"
# include "./type_id.hpp"
# include "./errors.hpp"

# include "./detail/mpl2/eval_if.hpp"
# include "./detail/mpl2/identity.hpp"

// opaque --
//
// registers to- and from- python conversions for a type Pointee.
//
// Note:
// In addition you need to define specializations for type_id
// on the type pointed to by Pointer using
// PXR_BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(Pointee)
//
// For an example see libs/python/test/opaque.cpp
//
namespace pxr { namespace boost { namespace python {

template <class Pointee>
struct opaque
{
    opaque()
    {
        if (type_object.tp_name == 0)
        {
            type_object.tp_name = const_cast<char*>(type_id<Pointee*>().name());
            if (PyType_Ready (&type_object) < 0)
            {
                throw error_already_set();
            }

            this->register_self();
        }
    }
    
    static opaque instance;
private:
    
    static void* extract(PyObject* op)
    {
        return PyObject_TypeCheck(op, &type_object)
            ? static_cast<python_instance*>(static_cast<void*>(op))->x
            : 0
            ;
    }

    static PyObject* wrap(void const* px)
    {
        Pointee* x = *static_cast<Pointee*const*>(px);
        
        if (x == 0)
            return detail::none();

        if ( python_instance *o = PyObject_New(python_instance, &type_object) )
        {
            o->x = x;
            return static_cast<PyObject*>(static_cast<void*>(o));
        }
        else
        {
            throw error_already_set();
        }
    }

    void register_self()
    {
        converter::registration const *existing =
            converter::registry::query (type_id<Pointee*>());

        if ((existing == 0) || (existing->m_to_python == 0))
        {
#ifndef PXR_BOOST_PYTHON_NO_PY_SIGNATURES
            converter::registry::insert(&extract, type_id<Pointee>(), &get_pytype);
            converter::registry::insert(&wrap, type_id<Pointee*>(), &get_pytype);
#else
            converter::registry::insert(&extract, type_id<Pointee>());
            converter::registry::insert(&wrap, type_id<Pointee*>());
#endif
        }
    }

    struct python_instance
    {
        PyObject_HEAD
        Pointee* x;
    };
    
    static PyTypeObject type_object;
#ifndef PXR_BOOST_PYTHON_NO_PY_SIGNATURES
    static PyTypeObject const *get_pytype(){return  &type_object; }
#endif
};

template <class Pointee>
opaque<Pointee> opaque<Pointee>::instance;

template <class Pointee>
PyTypeObject opaque<Pointee>::type_object =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    0,
    sizeof( typename opaque<Pointee>::python_instance ),
    0,
    ::pxr::boost::python::detail::dealloc,
    0,          /* tp_print */
    0,          /* tp_getattr */
    0,          /* tp_setattr */
    0,          /* tp_compare */
    0,          /* tp_repr */
    0,          /* tp_as_number */
    0,          /* tp_as_sequence */
    0,          /* tp_as_mapping */
    0,          /* tp_hash */
    0,          /* tp_call */
    0,          /* tp_str */
    0,          /* tp_getattro */
    0,          /* tp_setattro */
    0,          /* tp_as_buffer */
    0,          /* tp_flags */
    0,          /* tp_doc */
    0,          /* tp_traverse */
    0,          /* tp_clear */
    0,          /* tp_richcompare */
    0,          /* tp_weaklistoffset */
    0,          /* tp_iter */
    0,          /* tp_iternext */
    0,          /* tp_methods */
    0,          /* tp_members */
    0,          /* tp_getset */
    0,          /* tp_base */
    0,          /* tp_dict */
    0,          /* tp_descr_get */
    0,          /* tp_descr_set */
    0,          /* tp_dictoffset */
    0,          /* tp_init */
    0,          /* tp_alloc */
    0,          /* tp_new */
    0,          /* tp_free */
    0,          /* tp_is_gc */
    0,          /* tp_bases */
    0,          /* tp_mro */
    0,          /* tp_cache */
    0,          /* tp_subclasses */
    0,          /* tp_weaklist */
#if PYTHON_API_VERSION >= 1012
    0           /* tp_del */
#endif
};
}}} // namespace pxr::boost::python

// If you change the below, don't forget to alter the end of type_id.hpp
#   define PXR_BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(Pointee)                     \
    namespace pxr { namespace boost { namespace python {                        \
    template<>                                                                  \
    inline type_info type_id<Pointee>()                                         \
    {                                                                           \
        return type_info (typeid (Pointee *));                                  \
    }                                                                           \
    template<>                                                                  \
    inline type_info type_id<const volatile Pointee&>()                         \
    {                                                                           \
        return type_info (typeid (Pointee *));                                  \
    }                                                                           \
    }}}

# endif    // PXR_BOOST_PYTHON_OPAQUE_POINTER_CONVERTER_HPP

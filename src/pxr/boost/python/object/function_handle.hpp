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

#ifndef PXR_BOOST_PYTHON_OBJECT_FUNCTION_HANDLE_HPP
# define PXR_BOOST_PYTHON_OBJECT_FUNCTION_HANDLE_HPP

# include "../handle.hpp"
# include "../detail/caller.hpp"
# include "../default_call_policies.hpp"
# include "./py_function.hpp"
# include "../signature.hpp"

namespace pxr { namespace boost { namespace python { namespace objects {

PXR_BOOST_PYTHON_DECL handle<> function_handle_impl(py_function const& f);

// Just like function_object, but returns a handle<> instead. Using
// this for arg_to_python<> allows us to break a circular dependency
// between object and arg_to_python.
template <class F, class Signature>
inline handle<> function_handle(F const& f, Signature)
{
    enum { n_arguments = python::detail::mpl2::size<Signature>::value - 1 };

    return objects::function_handle_impl(
        python::detail::caller<
            F,default_call_policies,Signature
        >(
            f, default_call_policies()
         )
    );
}

// Just like make_function, but returns a handle<> intead. Same
// reasoning as above.
template <class F>
handle<> make_function_handle(F f)
{
    return objects::function_handle(f, python::detail::get_signature(f));
}

}}}} // namespace pxr::boost::python::objects

#endif // PXR_BOOST_PYTHON_OBJECT_FUNCTION_HANDLE_HPP

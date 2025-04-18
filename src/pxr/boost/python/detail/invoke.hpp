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

# ifndef PXR_BOOST_PYTHON_DETAIL_INVOKE_HPP
#  define PXR_BOOST_PYTHON_DETAIL_INVOKE_HPP

#  include "./prefix.hpp"
#  include "./preprocessor.hpp"
#  include "./none.hpp"
#  include "../to_python_value.hpp"

// This file declares a series of overloaded invoke(...)  functions,
// used to invoke wrapped C++ function (object)s from Python. Each one
// accepts:
//
//   - a tag which identifies the invocation syntax (e.g. member
//   functions must be invoked with a different syntax from regular
//   functions)
//
//   - a pointer to a result converter type, used solely as a way of
//   transmitting the type of the result converter to the function (or
//   an int, if the return type is void).
//
//   - the "function", which may be a function object, a function or
//   member function pointer, or a defaulted_virtual_fn.
//
//   - The arg_from_python converters for each of the arguments to be
//   passed to the function being invoked.

namespace pxr { namespace boost { namespace python { namespace detail {

// This "result converter" is really just used as a dispatch tag to
// invoke(...), selecting the appropriate implementation
typedef int void_result_to_python;

template <bool void_return, bool member>
struct invoke_tag_ {};

// A metafunction returning the appropriate tag type for invoking an
// object of type F with return type R.
template <class R, class F>
struct invoke_tag
  : invoke_tag_<
        is_same<R,void>::value
      , is_member_function_pointer<F>::value
    >
{
};

template <class RC, class F, class... AC>
inline PyObject* invoke(invoke_tag_<false,false>, RC const& rc, F& f, AC&... ac)
{
    return rc(f( ac()... ));
}

template <class RC, class F, class... AC>
inline PyObject* invoke(invoke_tag_<true,false>, RC const&, F& f, AC&... ac)
{
    f( ac()... );
    return none();
}

template <class RC, class F, class TC, class... AC>
inline PyObject* invoke(invoke_tag_<false,true>, RC const& rc, F& f, TC& tc, AC&... ac)
{
    return rc( (tc().*f)(ac()...) );
}
                 
template <class RC, class F, class TC, class... AC>
inline PyObject* invoke(invoke_tag_<true,true>, RC const&, F& f, TC& tc, AC&... ac)
{
    (tc().*f)(ac()...);
    return none();
}

}}}} // namespace pxr::boost::python::detail

# endif // PXR_BOOST_PYTHON_DETAIL_INVOKE_HPP

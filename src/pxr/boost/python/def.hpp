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

#ifndef PXR_BOOST_PYTHON_DEF_HPP
# define PXR_BOOST_PYTHON_DEF_HPP

# include "./detail/prefix.hpp"

# include "./object_fwd.hpp"
# include "./make_function.hpp"
# include "./detail/def_helper.hpp"
# include "./detail/overloads_fwd.hpp"
# include "./scope.hpp"
# include "./signature.hpp"
# include "./detail/scope.hpp"

namespace pxr { namespace boost { namespace python {

namespace detail
{
  namespace error
  {
    // Compile-time error messages
    template <bool> struct multiple_functions_passed_to_def;
    template <> struct multiple_functions_passed_to_def<false> { typedef char type; };
  }
  
  //
  // def_from_helper --
  //
  // Use a def_helper to define a regular wrapped function in the current scope.
  template <class F, class Helper>
  void def_from_helper(
      char const* name, F const& fn, Helper const& helper)
  {
      // Must not try to use default implementations except with method definitions.
      [[maybe_unused]] typedef typename error::multiple_functions_passed_to_def<
          Helper::has_default_implementation
          >::type assertion;
      
      detail::scope_setattr_doc(
          name, pxr::boost::python::make_function(
              fn
              , helper.policies()
              , helper.keywords())
          , helper.doc()
          );
  }

  //
  // These two overloads discriminate between def() as applied to
  // regular functions and def() as applied to the result of
  // PXR_BOOST_PYTHON_FUNCTION_OVERLOADS(). The final argument is used to
  // discriminate.
  //
  template <class Fn, class A1>
  void
  def_maybe_overloads(
      char const* name
      , Fn fn
      , A1 const& a1
      , ...)
  {
      detail::def_from_helper(name, fn, def_helper<A1>(a1));
  }

  template <class StubsT, class SigT>
  void def_maybe_overloads(
      char const* name
      , SigT sig
      , StubsT const& stubs
      , detail::overloads_base const*)
  {
      scope current;
      
      detail::define_with_defaults(
          name, stubs, current, detail::get_signature(sig));
  }

  template <class T>
  object make_function1(T fn, ...) { return make_function(fn); }

  inline
  object make_function1(object const& x, object const*) { return x; }
}

template <class Fn>
void def(char const* name, Fn fn)
{
    detail::scope_setattr_doc(name, detail::make_function1(fn, &fn), 0);
}

template <class Arg1T, class Arg2T>
void def(char const* name, Arg1T arg1, Arg2T const& arg2)
{
    detail::def_maybe_overloads(name, arg1, arg2, &arg2);
}

template <class F, class A1, class A2>
void def(char const* name, F f, A1 const& a1, A2 const& a2)
{
    detail::def_from_helper(name, f, detail::def_helper<A1,A2>(a1,a2));
}

template <class F, class A1, class A2, class A3>
void def(char const* name, F f, A1 const& a1, A2 const& a2, A3 const& a3)
{
    detail::def_from_helper(name, f, detail::def_helper<A1,A2,A3>(a1,a2,a3));
}

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_DEF_HPP

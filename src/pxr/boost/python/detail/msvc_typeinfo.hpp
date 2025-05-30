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

#ifndef PXR_BOOST_PYTHON_DETAIL_MSVC_TYPEINFO_HPP
# define PXR_BOOST_PYTHON_DETAIL_MSVC_TYPEINFO_HPP

#include <typeinfo>
#include "../type.hpp"

//
// Fix for icc's broken typeid() implementation which doesn't strip
// decoration. This fix doesn't handle cv-qualified array types. It
// could probably be done, but I haven't figured it out yet.
//

// Note: This file is badly named. It initially was MSVC specific, but was
// extended to cover intel too. Now the old version of MSVC is no longer
// supported, but the intel version is still supported.

# if defined(BOOST_INTEL_CXX_VERSION) && BOOST_INTEL_CXX_VERSION <= 700

namespace pxr { namespace boost { namespace python { namespace detail {

typedef std::type_info const& typeinfo;

template <class T>
static typeinfo typeid_nonref(T const volatile*) { return typeid(T); }

template <class T>
inline typeinfo typeid_ref_1(T&(*)())
{
    return detail::typeid_nonref((T*)0);
}

// A non-reference
template <class T>
inline typeinfo typeid_ref(type<T>*, T&(*)(type<T>))
{
    return detail::typeid_nonref((T*)0);
}

// A reference
template <class T>
inline typeinfo typeid_ref(type<T>*, ...)
{
    return detail::typeid_ref_1((T(*)())0);
}

#if defined(BOOST_MSVC) || (defined(__BORLANDC__) && !defined(BOOST_DISABLE_WIN32))
#   define BOOST_PYTT_DECL __cdecl
#else
#   define BOOST_PYTT_DECL /**/
#endif

template< typename T > T&(* is_ref_tester1(type<T>) )(type<T>) { return 0; }
inline char BOOST_PYTT_DECL is_ref_tester1(...) { return 0; }

template <class T>
inline typeinfo msvc_typeid(type<T>*)
{
    return detail::typeid_ref(
        (type<T>*)0, detail::is_ref_tester1(type<T>())
        );
}

template <>
inline typeinfo msvc_typeid<void>(type<void>*)
{
    return typeid(void);
}

#  ifndef NDEBUG
inline typeinfo assert_array_typeid_compiles()
{
    return msvc_typeid((type<char const[3]>*)0)
        , msvc_typeid((type<char[3]>*)0);
}
#  endif

}}}} // namespace pxr::boost::python::detail

# endif // BOOST_INTEL_CXX_VERSION
#endif // PXR_BOOST_PYTHON_DETAIL_MSVC_TYPEINFO_HPP

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

#ifndef PXR_BOOST_PYTHON_REFERENCE_EXISTING_OBJECT_HPP
# define PXR_BOOST_PYTHON_REFERENCE_EXISTING_OBJECT_HPP

# include "./detail/prefix.hpp"
# include "./detail/indirect_traits.hpp"
# include "./detail/mpl2/if.hpp"
# include "./to_python_indirect.hpp"
# include "./detail/type_traits.hpp"

namespace pxr { namespace boost { namespace python {

namespace detail
{
  template <class R>
  struct reference_existing_object_requires_a_pointer_or_reference_return_type
# if defined(__GNUC__) || defined(__EDG__)
  {}
# endif
  ;
}

template <class T> struct to_python_value;

struct reference_existing_object
{
    template <class T>
    struct apply
    {
        static constexpr 
            bool ok = detail::is_pointer<T>::value || detail::is_reference<T>::value;
        
        typedef typename python::detail::mpl2::if_c<
            ok
            , to_python_indirect<T, detail::make_reference_holder>
            , detail::reference_existing_object_requires_a_pointer_or_reference_return_type<T>
        >::type type;
    };
};

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_REFERENCE_EXISTING_OBJECT_HPP

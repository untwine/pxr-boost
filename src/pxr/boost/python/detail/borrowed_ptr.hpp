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

#ifndef PXR_BOOST_PYTHON_DETAIL_BORROWED_PTR_HPP
# define PXR_BOOST_PYTHON_DETAIL_BORROWED_PTR_HPP

# include "../type.hpp"
# include "./mpl2/if.hpp"
# include "./type_traits.hpp"
# include "../tag.hpp"

namespace pxr { namespace boost { namespace python { namespace detail {

template<class T> class borrowed
{ 
    typedef T type;
};

template<typename T>
struct is_borrowed_ptr
{
    static constexpr bool value = false; 
};

#  if !defined(__MWERKS__) || __MWERKS__ > 0x3000
template<typename T>
struct is_borrowed_ptr<borrowed<T>*>
{
    static constexpr bool value = true;
};

template<typename T>
struct is_borrowed_ptr<borrowed<T> const*>
{
    static constexpr bool value = true;
};

template<typename T>
struct is_borrowed_ptr<borrowed<T> volatile*>
{
    static constexpr bool value = true;
};

template<typename T>
struct is_borrowed_ptr<borrowed<T> const volatile*>
{
    static constexpr bool value = true;
};
#  else
template<typename T>
struct is_borrowed
{
    static constexpr bool value = false;
};
template<typename T>
struct is_borrowed<borrowed<T> >
{
    static constexpr bool value = true;
};
template<typename T>
struct is_borrowed_ptr<T*>
    : is_borrowed<typename remove_cv<T>::type>
{
};
#  endif 


}

template <class T>
inline T* get_managed_object(detail::borrowed<T> const volatile* p, tag_t)
{
    return (T*)p;
}

}}} // namespace pxr::boost::python::detail

#endif // #ifndef PXR_BOOST_PYTHON_DETAIL_BORROWED_PTR_HPP

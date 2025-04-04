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

#ifndef PXR_BOOST_PYTHON_TO_PYTHON_CONVERTER_HPP
# define PXR_BOOST_PYTHON_TO_PYTHON_CONVERTER_HPP

# include "./detail/prefix.hpp"

# include "./converter/registry.hpp"
# include "./converter/as_to_python_function.hpp"
#ifndef PXR_BOOST_PYTHON_NO_PY_SIGNATURES
# include "./converter/pytype_function.hpp"
#endif
# include "./type_id.hpp"
# include "./detail/mpl2/bool.hpp"

namespace pxr { namespace boost { namespace python {

#if 0 //get_pytype member detection
namespace detail
{
    typedef char yes_type;
    typedef struct {char a[2]; } no_type;
    template<PyTypeObject const * (*f)()> struct test_get_pytype1 { };
    template<PyTypeObject * (*f)()>          struct test_get_pytype2 { };

    template<class T> yes_type tester(test_get_pytype1<&T::get_pytype>*);

    template<class T> yes_type tester(test_get_pytype2<&T::get_pytype>*);

    template<class T> no_type tester(...);

    template<class T>
    struct test_get_pytype_base  
    {
        static constexpr bool value= (sizeof(detail::tester<T>(0) == sizeof(yes_type)));
    };

    template<class T>
    struct test_get_pytype : python::detail::mpl2::bool_<test_get_pytype_base<T>::value> 
    {
    };

}
#endif

template < class T, class Conversion, bool has_get_pytype=false >
struct to_python_converter 
{
#ifndef PXR_BOOST_PYTHON_NO_PY_SIGNATURES
    typedef python::detail::mpl2::bool_<has_get_pytype> HasGetPytype;

    static PyTypeObject const* get_pytype_1(pxr::boost::python::detail::mpl2::true_ *)
    {
        return Conversion::get_pytype();
    }

    static PyTypeObject const* get_pytype_1(pxr::boost::python::detail::mpl2::false_ *)
    {
        return 0;
    }
    static PyTypeObject const* get_pytype_impl()
    {
        return get_pytype_1((HasGetPytype*)0);
    }
#endif
    
    to_python_converter();
};

//
// implementation
//

template <class T, class Conversion ,bool has_get_pytype>
to_python_converter<T,Conversion, has_get_pytype>::to_python_converter()
{
    typedef converter::as_to_python_function<
        T, Conversion
        > normalized;
            
    converter::registry::insert(
        &normalized::convert
        , type_id<T>()
#ifndef PXR_BOOST_PYTHON_NO_PY_SIGNATURES
        , &get_pytype_impl
#endif
        );
}

}}} // namespace pxr::boost::python

#endif // PXR_BOOST_PYTHON_TO_PYTHON_CONVERTER_HPP


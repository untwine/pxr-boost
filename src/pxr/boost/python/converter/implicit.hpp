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

#ifndef PXR_BOOST_PYTHON_CONVERTER_IMPLICIT_HPP
# define PXR_BOOST_PYTHON_CONVERTER_IMPLICIT_HPP

# include "./rvalue_from_python_data.hpp"
# include "./registrations.hpp"
# include "./registered.hpp"

# include "../extract.hpp"

namespace pxr { namespace boost { namespace python { namespace converter {

template <class Source, class Target>
struct implicit
{
    static void* convertible(PyObject* obj)
    {
        // Find a converter which can produce a Source instance from
        // obj. The user has told us that Source can be converted to
        // Target, and instantiating construct() below, ensures that
        // at compile-time.
        return implicit_rvalue_convertible_from_python(obj, registered<Source>::converters)
            ? obj : 0;
    }
      
    static void construct(PyObject* obj, rvalue_from_python_stage1_data* data)
    {
        void* storage = ((rvalue_from_python_storage<Target>*)data)->storage.bytes;

        arg_from_python<Source> get_source(obj);
        [[maybe_unused]] bool convertible = get_source.convertible();
        assert(convertible);
        
        new (storage) Target(get_source());
        
        // record successful construction
        data->convertible = storage;
    }
};

}}}} // namespace pxr::boost::python::converter

#endif // PXR_BOOST_PYTHON_CONVERTER_IMPLICIT_HPP

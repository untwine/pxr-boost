// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
//  (C) Copyright Joel de Guzman 2003.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_SUITE_INDEXING_CONTAINER_UTILS_HPP
# define PXR_BOOST_PYTHON_SUITE_INDEXING_CONTAINER_UTILS_HPP

# include <utility>
# include "../../object.hpp"
# include "../../handle.hpp"
# include "../../extract.hpp"
# include "../../stl_iterator.hpp"

namespace pxr { namespace boost { namespace python { namespace container_utils {
        
    template <typename Container>
    void
    extend_container(Container& container, object l)
    {
        typedef typename Container::value_type data_type;
        
        //  l must be iterable
        for (auto i = stl_input_iterator<object>(l), 
                 e = stl_input_iterator<object>(); i != e; ++i)
        {
            object elem(*i);

            extract<data_type const&> x(elem);
            //  try if elem is an exact data_type type
            if (x.check())
            {
                container.push_back(x());
            }
            else
            {
                //  try to convert elem to data_type type
                extract<data_type> x(elem);
                if (x.check())
                {
                    container.push_back(x());
                }
                else
                {
                    PyErr_SetString(PyExc_TypeError, "Incompatible Data Type");
                    throw_error_already_set();
                }
            }
        }          
    }

}}}} // namespace pxr::boost::python::container_utils

#endif

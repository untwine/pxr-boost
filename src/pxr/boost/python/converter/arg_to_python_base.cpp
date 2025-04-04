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

#include "./arg_to_python_base.hpp"
#include "../errors.hpp"
#include "./registrations.hpp"
#include "../handle.hpp"
#include "../refcount.hpp"

namespace pxr { namespace boost { namespace python { namespace converter {

namespace detail
{
  arg_to_python_base::arg_to_python_base(
      void const volatile* source, registration const& converters)
      : handle<>
         (converters.to_python(source))
  {
  }
}

}}}} // namespace pxr::boost::python::converter

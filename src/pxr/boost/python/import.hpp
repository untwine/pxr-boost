// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright Stefan Seefeld 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_IMPORT_HPP
# define PXR_BOOST_PYTHON_IMPORT_HPP

#ifndef PXR_USE_INTERNAL_BOOST_PYTHON
#include <boost/python/import.hpp>
#else

# include "./object.hpp"
# include "./str.hpp"

namespace pxr
{ 
namespace boost
{
namespace python
{

// Import the named module and return a reference to it.
object PXR_BOOST_PYTHON_DECL import(str name);

}
}
}

#endif // PXR_USE_INTERNAL_BOOST_PYTHON
#endif

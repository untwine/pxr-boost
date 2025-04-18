// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#ifndef PXR_BOOST_PYTHON_DETAIL_PREFIX_HPP
# define PXR_BOOST_PYTHON_DETAIL_PREFIX_HPP

// The rule is that <Python.h> must be included before any system
// headers (so it can get control over some awful macros).
// Unfortunately, Boost.Python needs to #include <limits.h> first, at
// least... but this gets us as close as possible.

# include "./wrap_python.hpp"
# include "./config.hpp"

#endif // PXR_BOOST_PYTHON_DETAIL_PREFIX_HPP

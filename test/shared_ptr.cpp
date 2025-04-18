// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Copyright David Abrahams 2002.
// Copyright Stefan Seefeld 2016.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include <pxr/boost/python/module.hpp>
#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/call_method.hpp>
#include <pxr/boost/python/extract.hpp>
#include <pxr/boost/python/def.hpp>
#include <memory>

using std::shared_ptr;
#define MODULE shared_ptr_ext

#include "shared_ptr.hpp"
#include "module_tail.cpp"


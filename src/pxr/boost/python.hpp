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

//  See http://www.boost.org/libs/python for documentation.

#ifndef PXR_BOOST_PYTHON_HPP
# define PXR_BOOST_PYTHON_HPP

#include "pxr/boost/python/common.hpp"

# include "pxr/boost/python/args.hpp"
# include "pxr/boost/python/args_fwd.hpp"
# include "pxr/boost/python/back_reference.hpp"
# include "pxr/boost/python/bases.hpp"
# include "pxr/boost/python/borrowed.hpp"
# include "pxr/boost/python/call.hpp"
# include "pxr/boost/python/call_method.hpp"
# include "pxr/boost/python/class.hpp"
# include "pxr/boost/python/copy_const_reference.hpp"
# include "pxr/boost/python/copy_non_const_reference.hpp"
# include "pxr/boost/python/data_members.hpp"
# include "pxr/boost/python/def.hpp"
# include "pxr/boost/python/default_call_policies.hpp"
# include "pxr/boost/python/dict.hpp"
# include "pxr/boost/python/docstring_options.hpp"
# include "pxr/boost/python/enum.hpp"
# include "pxr/boost/python/errors.hpp"
# include "pxr/boost/python/exception_translator.hpp"
# include "pxr/boost/python/exec.hpp"
# include "pxr/boost/python/extract.hpp"
# include "pxr/boost/python/handle.hpp"
# include "pxr/boost/python/has_back_reference.hpp"
# include "pxr/boost/python/implicit.hpp"
# include "pxr/boost/python/init.hpp"
# include "pxr/boost/python/import.hpp"
# include "pxr/boost/python/instance_holder.hpp"
# include "pxr/boost/python/iterator.hpp"
# include "pxr/boost/python/list.hpp"
# include "pxr/boost/python/long.hpp"
# include "pxr/boost/python/lvalue_from_pytype.hpp"
# include "pxr/boost/python/make_constructor.hpp"
# include "pxr/boost/python/make_function.hpp"
# include "pxr/boost/python/manage_new_object.hpp"
# include "pxr/boost/python/module.hpp"
# include "pxr/boost/python/noncopyable.hpp"
# include "pxr/boost/python/object.hpp"
# include "pxr/boost/python/object_protocol.hpp"
# include "pxr/boost/python/object_protocol_core.hpp"
# include "pxr/boost/python/opaque_pointer_converter.hpp"
# include "pxr/boost/python/operators.hpp"
# include "pxr/boost/python/other.hpp"
# include "pxr/boost/python/overloads.hpp"
# include "pxr/boost/python/pointee.hpp"
# include "pxr/boost/python/pure_virtual.hpp"
# include "pxr/boost/python/ptr.hpp"
# include "pxr/boost/python/raw_function.hpp"
# include "pxr/boost/python/ref.hpp"
# include "pxr/boost/python/reference_existing_object.hpp"
# include "pxr/boost/python/register_ptr_to_python.hpp"
# include "pxr/boost/python/return_arg.hpp"
# include "pxr/boost/python/return_internal_reference.hpp"
# include "pxr/boost/python/return_opaque_pointer.hpp"
# include "pxr/boost/python/return_value_policy.hpp"
# include "pxr/boost/python/scope.hpp"
# include "pxr/boost/python/self.hpp"
# include "pxr/boost/python/slice.hpp"
# include "pxr/boost/python/slice_nil.hpp"
# include "pxr/boost/python/stl_iterator.hpp"
# include "pxr/boost/python/str.hpp"
# include "pxr/boost/python/to_python_converter.hpp"
# include "pxr/boost/python/to_python_indirect.hpp"
# include "pxr/boost/python/to_python_value.hpp"
# include "pxr/boost/python/tuple.hpp"
# include "pxr/boost/python/type.hpp"
# include "pxr/boost/python/type_id.hpp"
# include "pxr/boost/python/type_list.hpp"
# include "pxr/boost/python/with_custodian_and_ward.hpp"

#endif // PXR_BOOST_PYTHON_HPP

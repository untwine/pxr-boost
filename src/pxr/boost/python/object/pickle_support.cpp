// Copyright 2024 Pixar
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
//  (C) Copyright R.W. Grosse-Kunstleve 2002.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Modified by Jeremy Retailleau.

#include "../make_function.hpp"
#include "./class.hpp"
#include "../tuple.hpp"
#include "../list.hpp"
#include "../dict.hpp"
#include "../str.hpp"

namespace pxr { namespace boost { namespace python {

namespace {

  tuple instance_reduce(object instance_obj)
  {
      list result;
      object instance_class(instance_obj.attr("__class__"));
      result.append(instance_class);
      object none;
      if (!getattr(instance_obj, "__safe_for_unpickling__", none))
      {
          str type_name(getattr(instance_class, "__name__"));
          str module_name(getattr(instance_class, "__module__", object("")));
          if (module_name)
              module_name += ".";

          PyErr_SetObject(
               PyExc_RuntimeError,
               ( "Pickling of \"%s\" instances is not enabled"
                 " (http://www.boost.org/libs/python/doc/v2/pickle.html)"
                  % (module_name+type_name)).ptr()
          );

          throw_error_already_set();
      }
      object getinitargs = getattr(instance_obj, "__getinitargs__", none);
      tuple initargs;
      if (!getinitargs.is_none()) {
          initargs = tuple(getinitargs());
      }
      result.append(initargs);
      object getstate = getattr(instance_obj, "__getstate__", none);

      // Python 3.11 added a default implementation of __getstate__ to
      // object, which we need to ignore to maintain previous behavior.
      // 
      // For example, if a class had pickle support enabled but did not
      // provide __getstate__, instances with items directly added to their
      // __dict__ would skip the __getstate_manages_dict__ safeguard below
      // prior to 3.11. After 3.11, the safeguard would be triggered.
#if PY_VERSION_HEX >= 0x030b00f0
      if (!getstate.is_none()) {
          object class_getstate = getattr(instance_class, "__getstate__", none);
          handle<> obj_getstate(PyObject_GetAttrString(
              (PyObject*)&PyBaseObject_Type, "__getstate__"));
          if (class_getstate.ptr() == obj_getstate.get()) {
              getstate = none;
          }
      }
#endif

      object instance_dict = getattr(instance_obj, "__dict__", none);
      long len_instance_dict = 0;
      if (!instance_dict.is_none()) {
          len_instance_dict = len(instance_dict);
      }
      if (!getstate.is_none()) {
          if (len_instance_dict > 0) {
              object getstate_manages_dict = getattr(
                instance_obj, "__getstate_manages_dict__", none);
              if (getstate_manages_dict.is_none()) {
                  PyErr_SetString(PyExc_RuntimeError,
                    "Incomplete pickle support"
                    " (__getstate_manages_dict__ not set)");
                  throw_error_already_set();
              }
          }
          result.append(getstate());
      }
      else if (len_instance_dict > 0) {
          result.append(instance_dict);
      }
      return tuple(result);
  }

} // namespace

object const& make_instance_reduce_function()
{
    static object result(&instance_reduce);
    return result;
}

}}} // namespace pxr::boost::python

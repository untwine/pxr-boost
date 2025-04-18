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

#include "../handle.hpp"
#include "../type_id.hpp"
#include "../errors.hpp"
#include "../refcount.hpp"

#include "../detail/config.hpp"
#include "../detail/integer_cast.hpp"
#include "../detail/wrap_python.hpp"

#include "./builtin_converters.hpp"
#include "./rvalue_from_python_data.hpp"
#include "./registry.hpp"
#include "./registrations.hpp"
#include "./shared_ptr_deleter.hpp"
#include "./pytype_function.hpp"

#include <string>
#include <complex>
#include <type_traits>

namespace pxr { namespace boost { namespace python { namespace converter {

shared_ptr_deleter::shared_ptr_deleter(handle<> owner)
    : owner(owner)
{}

shared_ptr_deleter::~shared_ptr_deleter() {}

namespace
{

  class scoped_ensure_gil_state
  {
  public:
    scoped_ensure_gil_state()
        : m_gil_state(PyGILState_Ensure())
    {}

    ~scoped_ensure_gil_state()
    {
        PyGILState_Release(m_gil_state);
    }

  private:
      PyGILState_STATE m_gil_state;
  };

}

void shared_ptr_deleter::operator()(void const*)
{
    scoped_ensure_gil_state gil;
    owner.reset();
}

namespace
{

  // An lvalue conversion function which extracts a char const* from a
  // Python String.
#if PY_VERSION_HEX < 0x03000000
  void* convert_to_cstring(PyObject* obj)
  {
      return PyString_Check(obj) ? PyString_AsString(obj) : 0;
  }
#elif PY_VERSION_HEX < 0x03070000
  void* convert_to_cstring(PyObject* obj)
  {
      return PyUnicode_Check(obj) ? _PyUnicode_AsString(obj) : 0;
  }
#else
  void* convert_to_cstring(PyObject* obj)
  {
      return PyUnicode_Check(obj) ? const_cast<void*>(reinterpret_cast<const void*>(_PyUnicode_AsString(obj))) : 0;
  }
#endif

  // Given a target type and a SlotPolicy describing how to perform a
  // given conversion, registers from_python converters which use the
  // SlotPolicy to extract the type.
  template <class T, class SlotPolicy>
  struct slot_rvalue_from_python
  {
   public:
      slot_rvalue_from_python()
      {
          registry::insert(
              &slot_rvalue_from_python<T,SlotPolicy>::convertible
              , &slot_rvalue_from_python<T,SlotPolicy>::construct
              , type_id<T>()
              , &SlotPolicy::get_pytype
              );
      }
      
   private:
      static void* convertible(PyObject* obj)
      {
          unaryfunc* slot = SlotPolicy::get_slot(obj);
          return slot && *slot ? slot : 0;
      }

      static void construct(PyObject* obj, rvalue_from_python_stage1_data* data)
      {
          // Get the (intermediate) source object
          unaryfunc creator = *static_cast<unaryfunc*>(data->convertible);
          handle<> intermediate(creator(obj));

          // Get the location in which to construct
          void* storage = ((rvalue_from_python_storage<T>*)data)->storage.bytes;
# ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable:4244)
# endif 
          new (storage) T( SlotPolicy::extract(intermediate.get()) );
          
# ifdef _MSC_VER
#  pragma warning(pop)
# endif 
          // record successful construction
          data->convertible = storage;
      }
  };

  // identity_unaryfunc/py_object_identity -- manufacture a unaryfunc
  // "slot" which just returns its argument. 
  extern "C" PyObject* identity_unaryfunc(PyObject* x)
  {
      Py_INCREF(x);
      return x;
  }
  unaryfunc py_object_identity = identity_unaryfunc;

#if PY_VERSION_HEX >= 0x03000000
  // As in Python 3 there is only one integer type, we can have much
  // simplified logic.
  // XXX(bhy) maybe the code will work with 2.6 or even 2.5?
  struct int_rvalue_from_python_base
  {
      static unaryfunc* get_slot(PyObject* obj)
      {
          return PyLong_Check(obj) ? &py_object_identity : 0;
      }
      static PyTypeObject const* get_pytype() {return &PyLong_Type;}
  };

  template <class T>
  struct signed_int_rvalue_from_python : int_rvalue_from_python_base
  {
      static_assert(!std::is_floating_point<T>::value);

      static T extract(PyObject* intermediate)
      {
          long x = PyLong_AsLong(intermediate);
          if (PyErr_Occurred())
              throw_error_already_set();
          return python::detail::integer_cast<T>(x);
      }
  };

  template <class T>
  struct unsigned_int_rvalue_from_python : int_rvalue_from_python_base
  {
      static_assert(!std::is_floating_point<T>::value);

      static T extract(PyObject* intermediate)
      {
          unsigned long x = PyLong_AsUnsignedLong(intermediate);
          if (PyErr_Occurred())
              throw_error_already_set();
          return python::detail::integer_cast<T>(x);
      }
  };
#else // PY_VERSION_HEX >= 0x03000000
  // A SlotPolicy for extracting signed integer types from Python objects
  struct signed_int_rvalue_from_python_base
  {
      static unaryfunc* get_slot(PyObject* obj)
      {
          PyNumberMethods* number_methods = obj->ob_type->tp_as_number;
          if (number_methods == 0)
              return 0;

          return (
#if PY_VERSION_HEX >= 0x02040000 && defined(PXR_BOOST_PYTHON_BOOL_INT_STRICT)
          !PyBool_Check(obj) &&
#endif
          (PyInt_Check(obj) || PyLong_Check(obj)))

        ? &number_methods->nb_int : 0;
      }
      static PyTypeObject const* get_pytype() { return &PyInt_Type;}
  };

  template <class T>
  struct signed_int_rvalue_from_python : signed_int_rvalue_from_python_base
  {
      static_assert(!std::is_floating_point<T>::value);

      static T extract(PyObject* intermediate)
      {
          long x = PyInt_AsLong(intermediate);
          if (PyErr_Occurred())
              throw_error_already_set();
          return python::detail::integer_cast<T>(x);
      }
  };
  
  // A SlotPolicy for extracting unsigned integer types from Python objects
  struct unsigned_int_rvalue_from_python_base
  {
      static unaryfunc* get_slot(PyObject* obj)
      {
          PyNumberMethods* number_methods = obj->ob_type->tp_as_number;
          if (number_methods == 0)
              return 0;

          return (
#if PY_VERSION_HEX >= 0x02040000 && defined(PXR_BOOST_PYTHON_BOOL_INT_STRICT)
          !PyBool_Check(obj) &&
#endif
          (PyInt_Check(obj) || PyLong_Check(obj)))
              ? &py_object_identity : 0;
      }
      static PyTypeObject const* get_pytype() { return &PyInt_Type;}
  };

  template <class T>
  struct unsigned_int_rvalue_from_python : unsigned_int_rvalue_from_python_base
  {
      static_assert(!std::is_floating_point<T>::value);

      static T extract(PyObject* intermediate)
      {
          if (PyLong_Check(intermediate)) {
              // PyLong_AsUnsignedLong() checks for negative overflow, so no
              // need to check it here.
              unsigned long result = PyLong_AsUnsignedLong(intermediate);
              if (PyErr_Occurred())
                  throw_error_already_set();
              return python::detail::integer_cast<T>(x);
          } else {
              // None of PyInt_AsUnsigned*() functions check for negative
              // overflow, so use PyInt_AS_LONG instead and check if number is
              // negative, issuing the exception appropriately.
              long result = PyInt_AS_LONG(intermediate);
              if (PyErr_Occurred())
                  throw_error_already_set();
              if (result < 0) {
                  PyErr_SetString(PyExc_OverflowError, "can't convert negative"
                                  " value to unsigned");
                  throw_error_already_set();
              }
              return python::detail::integer_cast<T>(x);
          }
      }
  };
#endif // PY_VERSION_HEX >= 0x03000000

// Checking Python's macro instead of Boost's - we don't seem to get
// the config right all the time. Furthermore, Python's is defined
// when long long is absent but __int64 is present.
  
#ifdef HAVE_LONG_LONG
  // A SlotPolicy for extracting long long types from Python objects

  struct long_long_rvalue_from_python_base
  {
      static unaryfunc* get_slot(PyObject* obj)
      {
#if PY_VERSION_HEX >= 0x03000000
          return PyLong_Check(obj) ? &py_object_identity : 0;
#else
          PyNumberMethods* number_methods = obj->ob_type->tp_as_number;
          if (number_methods == 0)
              return 0;

          // Return the identity conversion slot to avoid creating a
          // new object. We'll handle that in the extract function
          if (PyInt_Check(obj))
              return &number_methods->nb_int;
          else if (PyLong_Check(obj))
              return &number_methods->nb_long;
          else
              return 0;
#endif
      }
      static PyTypeObject const* get_pytype() { return &PyLong_Type;}
  };
  
  struct long_long_rvalue_from_python : long_long_rvalue_from_python_base
  {
      static PXR_BOOST_PYTHON_LONG_LONG extract(PyObject* intermediate)
      {
#if PY_VERSION_HEX < 0x03000000
          if (PyInt_Check(intermediate))
          {
              return PyInt_AS_LONG(intermediate);
          }
          else
#endif
          {
              PXR_BOOST_PYTHON_LONG_LONG result = PyLong_AsLongLong(intermediate);
              
              if (PyErr_Occurred())
                  throw_error_already_set();

              return result;
          }
      }
  };

  struct unsigned_long_long_rvalue_from_python : long_long_rvalue_from_python_base
  {
      static unsigned PXR_BOOST_PYTHON_LONG_LONG extract(PyObject* intermediate)
      {
#if PY_VERSION_HEX < 0x03000000
          if (PyInt_Check(intermediate))
          {
              return python::detail::integer_cast<unsigned PXR_BOOST_PYTHON_LONG_LONG>(PyInt_AS_LONG(intermediate));
          }
          else
#endif
          {
              unsigned PXR_BOOST_PYTHON_LONG_LONG result = PyLong_AsUnsignedLongLong(intermediate);
              
              if (PyErr_Occurred())
                  throw_error_already_set();

              return result;
          }
      }
  };
#endif 

  // A SlotPolicy for extracting bool from a Python object
  struct bool_rvalue_from_python
  {
      static unaryfunc* get_slot(PyObject* obj)
      {
#if PY_VERSION_HEX >= 0x03000000
          return obj == Py_None || PyLong_Check(obj) ? &py_object_identity : 0;
#elif PY_VERSION_HEX >= 0x02040000 && defined(PXR_BOOST_PYTHON_BOOL_INT_STRICT)
          return obj == Py_None || PyBool_Check(obj) ? &py_object_identity : 0;
#else
          return obj == Py_None || PyInt_Check(obj) ? &py_object_identity : 0;
#endif
      }
      
      static bool extract(PyObject* intermediate)
      {
          return PyObject_IsTrue(intermediate);
      }

      static PyTypeObject const* get_pytype()
      {
#if PY_VERSION_HEX >= 0x02030000
        return &PyBool_Type;
#else
        return &PyInt_Type;
#endif
      }
  };

  // A SlotPolicy for extracting floating types from Python objects.
  struct float_rvalue_from_python
  {
      static unaryfunc* get_slot(PyObject* obj)
      {
          PyNumberMethods* number_methods = obj->ob_type->tp_as_number;
          if (number_methods == 0)
              return 0;

          // For integer types, return the tp_int conversion slot to avoid
          // creating a new object. We'll handle that below
#if PY_VERSION_HEX < 0x03000000
          if (PyInt_Check(obj))
              return &number_methods->nb_int;
#endif

          return (PyLong_Check(obj) || PyFloat_Check(obj))
              ? &number_methods->nb_float : 0;
      }
      
      static double extract(PyObject* intermediate)
      {
#if PY_VERSION_HEX < 0x03000000
          if (PyInt_Check(intermediate))
          {
              return PyInt_AS_LONG(intermediate);
          }
          else
#endif
          {
              return PyFloat_AS_DOUBLE(intermediate);
          }
      }
      static PyTypeObject const* get_pytype() { return &PyFloat_Type;}
  };

#if PY_VERSION_HEX >= 0x03000000
  unaryfunc py_unicode_as_string_unaryfunc = PyUnicode_AsUTF8String;
#endif

  // A SlotPolicy for extracting C++ strings from Python objects.
  struct string_rvalue_from_python
  {
      // If the underlying object is "string-able" this will succeed
      static unaryfunc* get_slot(PyObject* obj)
      {
#if PY_VERSION_HEX >= 0x03000000
          return (PyUnicode_Check(obj)) ? &py_unicode_as_string_unaryfunc : 
                  PyBytes_Check(obj) ? &py_object_identity : 0;
#else
          return (PyString_Check(obj)) ? &obj->ob_type->tp_str : 0;

#endif
      };

      // Remember that this will be used to construct the result object 
#if PY_VERSION_HEX >= 0x03000000
      static std::string extract(PyObject* intermediate)
      {
          return std::string(PyBytes_AsString(intermediate),PyBytes_Size(intermediate));
      }
      static PyTypeObject const* get_pytype() { return &PyUnicode_Type;}
#else
      static std::string extract(PyObject* intermediate)
      {
          return std::string(PyString_AsString(intermediate),PyString_Size(intermediate));
      }
      static PyTypeObject const* get_pytype() { return &PyString_Type;}
#endif
  };

#if defined(Py_USING_UNICODE)
  // encode_string_unaryfunc/py_encode_string -- manufacture a unaryfunc
  // "slot" which encodes a Python string using the default encoding
  extern "C" PyObject* encode_string_unaryfunc(PyObject* x)
  {
      return PyUnicode_FromEncodedObject( x, 0, 0 );
  }
  unaryfunc py_encode_string = encode_string_unaryfunc;

  // A SlotPolicy for extracting C++ strings from Python objects.
  struct wstring_rvalue_from_python
  {
      // If the underlying object is "string-able" this will succeed
      static unaryfunc* get_slot(PyObject* obj)
      {
          return PyUnicode_Check(obj)
              ? &py_object_identity
#if PY_VERSION_HEX >= 0x03000000
            : PyBytes_Check(obj)
#else
            : PyString_Check(obj)
#endif
              ? &py_encode_string
            : 0;
      };

      // Remember that this will be used to construct the result object 
      static std::wstring extract(PyObject* intermediate)
      {
          // On Windows, with Python >= 3.3, PyObject_Length cannot be used to get
          // the size of the wchar_t string, because it will count the number of
          // *code points*, but some characters not on the BMP will use two UTF-16
          // *code units* (surrogate pairs).
          // This is not a problem on Unix, since wchar_t is 32-bit.
#if defined(_WIN32) && PY_VERSION_HEX >= 0x03030000
          static_assert(sizeof(wchar_t) == 2);

          Py_ssize_t size = 0;
          wchar_t *buf = PyUnicode_AsWideCharString(intermediate, &size);
          if (buf == NULL) {
              pxr::boost::python::throw_error_already_set();
          }
          std::wstring result(buf, size);
          PyMem_Free(buf);
#else
          std::wstring result(::PyObject_Length(intermediate), L' ');
          if (!result.empty())
          {
              int err = PyUnicode_AsWideChar(
#if PY_VERSION_HEX < 0x03020000
                  (PyUnicodeObject *)
#endif
                    intermediate
                , &result[0]
                , result.size());

              if (err == -1)
                  throw_error_already_set();
          }
#endif
          return result;
      }
      static PyTypeObject const* get_pytype() { return &PyUnicode_Type;}
  };
#endif 

  struct complex_rvalue_from_python
  {
      static unaryfunc* get_slot(PyObject* obj)
      {
          if (PyComplex_Check(obj))
              return &py_object_identity;
          else
              return float_rvalue_from_python::get_slot(obj);
      }
      
      static std::complex<double> extract(PyObject* intermediate)
      {
          if (PyComplex_Check(intermediate))
          {
              return std::complex<double>(
                  PyComplex_RealAsDouble(intermediate)
                  , PyComplex_ImagAsDouble(intermediate));
          }
#if PY_VERSION_HEX < 0x03000000
          else if (PyInt_Check(intermediate))
          {
              return PyInt_AS_LONG(intermediate);
          }
#endif
          else
          {
              return PyFloat_AS_DOUBLE(intermediate);
          }
      }
      static PyTypeObject const* get_pytype() { return &PyComplex_Type;}
  };
} 

PXR_BOOST_PYTHON_DECL PyObject* do_return_to_python(char x)
{
#if PY_VERSION_HEX >= 0x03000000
    return PyUnicode_FromStringAndSize(&x, 1);
#else
    return PyString_FromStringAndSize(&x, 1);
#endif
}
  
PXR_BOOST_PYTHON_DECL PyObject* do_return_to_python(char const* x)
{
#if PY_VERSION_HEX >= 0x03000000
    return x ? PyUnicode_FromString(x) : pxr::boost::python::detail::none();
#else
    return x ? PyString_FromString(x) : pxr::boost::python::detail::none();
#endif
}
  
PXR_BOOST_PYTHON_DECL PyObject* do_return_to_python(PyObject* x)
{
    return x ? x : pxr::boost::python::detail::none();
}
  
PXR_BOOST_PYTHON_DECL PyObject* do_arg_to_python(PyObject* x)
{
    if (x == 0)
        return pxr::boost::python::detail::none();
      
    Py_INCREF(x);
    return x;
}

#define REGISTER_INT_CONVERTERS(signedness, U)                          \
        slot_rvalue_from_python<                                        \
                signedness U                                            \
                ,signedness##_int_rvalue_from_python<signedness U>      \
         >()

#define REGISTER_INT_CONVERTERS2(U)             \
        REGISTER_INT_CONVERTERS(signed, U);     \
        REGISTER_INT_CONVERTERS(unsigned, U)  

void initialize_builtin_converters()
{
    // booleans
    slot_rvalue_from_python<bool,bool_rvalue_from_python>();

    // integer types
    REGISTER_INT_CONVERTERS2(char);
    REGISTER_INT_CONVERTERS2(short);
    REGISTER_INT_CONVERTERS2(int);
    REGISTER_INT_CONVERTERS2(long);
    
// using Python's macro instead of Boost's - we don't seem to get the
// config right all the time.
# ifdef HAVE_LONG_LONG
    slot_rvalue_from_python<signed PXR_BOOST_PYTHON_LONG_LONG,long_long_rvalue_from_python>();
    slot_rvalue_from_python<unsigned PXR_BOOST_PYTHON_LONG_LONG,unsigned_long_long_rvalue_from_python>();
# endif
        
    // floating types
    slot_rvalue_from_python<float,float_rvalue_from_python>();
    slot_rvalue_from_python<double,float_rvalue_from_python>();
    slot_rvalue_from_python<long double,float_rvalue_from_python>();
    
    slot_rvalue_from_python<std::complex<float>,complex_rvalue_from_python>();
    slot_rvalue_from_python<std::complex<double>,complex_rvalue_from_python>();
    slot_rvalue_from_python<std::complex<long double>,complex_rvalue_from_python>();
    
    // Add an lvalue converter for char which gets us char const*
#if PY_VERSION_HEX < 0x03000000
    registry::insert(convert_to_cstring,type_id<char>(),&converter::wrap_pytype<&PyString_Type>::get_pytype);
#else
    registry::insert(convert_to_cstring,type_id<char>(),&converter::wrap_pytype<&PyUnicode_Type>::get_pytype);
#endif

    // Register by-value converters to std::string, std::wstring
#if defined(Py_USING_UNICODE)
    slot_rvalue_from_python<std::wstring, wstring_rvalue_from_python>();
# endif 
    slot_rvalue_from_python<std::string, string_rvalue_from_python>();

}

}}}} // namespace pxr::boost::python::converter

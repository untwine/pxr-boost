# Create the test Python library with the necessary configurations.
function(add_python_library NAME)
    add_library(${NAME} SHARED ${ARGN})
    target_link_libraries(${NAME} PUBLIC boost-python)

    if(WIN32)
        set_target_properties(${NAME} PROPERTIES SUFFIX ".pyd")
    elseif(APPLE)
        set_target_properties(${NAME} PROPERTIES SUFFIX ".so")
    endif()

    set_target_properties(${NAME} PROPERTIES PREFIX "")
endfunction()

# Add a Python test with configurable script and dependencies
function(add_python_test NAME)
    cmake_parse_arguments(PARSE_ARGV 1 "" "" "SCRIPT" "DEPENDS")

    if (NOT _SCRIPT)
        set(_SCRIPT "${NAME}.py")
    endif()

    if (NOT _DEPENDS)
        set(_DEPENDS "${NAME}_ext")
    endif()

    add_test(NAME "${NAME}"
        COMMAND ${Python_EXECUTABLE}
        "${CMAKE_CURRENT_SOURCE_DIR}/pyrun.py" "${_SCRIPT}")

    set_test_environment(${NAME} DEPENDS ${_DEPENDS})
endfunction()

# Add a C++ test with optional script support.
function(add_cpp_test NAME)
    cmake_parse_arguments(PARSE_ARGV 1 "" "" "SCRIPT" "")

    set(COMMAND_ARGS "")
    if (_SCRIPT)
        set(COMMAND_ARGS "${CMAKE_CURRENT_SOURCE_DIR}/${_SCRIPT}")
    endif()

    add_test(NAME ${NAME} COMMAND ${NAME} ${COMMAND_ARGS})
    set_test_environment(${NAME} DEPENDS ${NAME})
endfunction()

# Set environment for a specific test.
function(set_test_environment NAME MODULE_NAMES)
    cmake_parse_arguments(PARSE_ARGV 1 "" "" "" "DEPENDS")

    set(PYTHONPATH "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PATH "")

    foreach(_name ${_DEPENDS})
        list(PREPEND PYTHONPATH "$<TARGET_FILE_DIR:${_name}>" ${PYTHONPATH})

        # Extend the DLL search path on Windows
        if (WIN32)
            set(PATH $<TARGET_RUNTIME_DLL_DIRS:${_name}>)
        endif()
    endforeach()

    set(SEP $<IF:$<BOOL:${WIN32}>,\\\\;,:>)
    set(ENVIRONMENT "PYTHONPATH=$<JOIN:$<SHELL_PATH:${PYTHONPATH}>,${SEP}>")
    if (NOT PATH STREQUAL "")
        list(APPEND ENVIRONMENT "PATH=$<JOIN:$<SHELL_PATH:${PATH}>,${SEP}>")
    endif()

    set_tests_properties(${NAME} PROPERTIES ENVIRONMENT "${ENVIRONMENT}")
endfunction()

add_python_library(injected_ext injected.cpp)
add_python_test(injected)

add_python_library(properties_ext properties.cpp)
add_python_test(properties)

add_python_library(return_arg_ext return_arg.cpp)
add_python_test(return_arg)

add_python_library(staticmethod_ext staticmethod.cpp)
add_python_test(staticmethod)

add_python_library(shared_ptr_ext shared_ptr.cpp)
add_python_test(shared_ptr)

add_python_library(enable_shared_from_this_ext enable_shared_from_this.cpp)
add_python_test(enable_shared_from_this)

add_python_library(andreas_beyer_ext andreas_beyer.cpp)
add_python_test(andreas_beyer)

add_python_library(wrapper_held_type_ext wrapper_held_type.cpp)
add_python_test(wrapper_held_type)

add_python_library(polymorphism_ext polymorphism.cpp)
add_python_test(polymorphism)

add_python_library(polymorphism2_ext polymorphism2.cpp)
add_python_test(polymorphism2)

add_python_library(minimal_ext minimal.cpp)
add_python_test(minimal)

add_python_library(args_ext args.cpp)
add_python_test(args)

add_python_library(raw_ctor_ext raw_ctor.cpp)
add_python_test(raw_ctor)

add_python_library(enum_ext enum_ext.cpp)
add_python_test(enum SCRIPT test_enum.py)

add_python_library(exception_translator_ext exception_translator.cpp)
add_python_test(exception_translator)

add_python_library(cltree cltree.cpp)
add_python_test(pearu1 DEPENDS cltree SCRIPT test_cltree.py)

add_python_library(m1 m1.cpp)
add_python_library(m2 m2.cpp)
add_python_test(try DEPENDS m1 m2 SCRIPT newtest.py)

add_python_library(const_argument_ext const_argument.cpp)
add_python_test(const_argument)

add_python_library(keywords keywords.cpp)
add_python_test(keywords DEPENDS keywords SCRIPT keywords_test.py)

add_python_library(builtin_converters_ext builtin_converters.cpp)
add_python_test(builtin_converters SCRIPT test_builtin_converters.py)

add_python_library(test_pointer_adoption_ext test_pointer_adoption.cpp)
add_python_test(test_pointer_adoption)

add_python_library(operators_ext operators.cpp)
add_python_test(operators)

add_python_library(operators_wrapper_ext operators_wrapper.cpp)
add_python_test(operators_wrapper)

add_python_library(callbacks_ext callbacks.cpp)
add_python_test(callbacks)

add_python_library(defaults_ext defaults.cpp)
add_python_test(defaults)

add_python_library(object_ext object.cpp)
add_python_test(object)

add_python_library(class_ext class.cpp)
add_python_test(class)

add_python_library(aligned_class_ext aligned_class.cpp)
add_python_test(aligned_class)

add_python_library(list_ext list.cpp)
add_python_test(list)

add_python_library(long_ext long.cpp)
add_python_test(long)

add_python_library(dict_ext dict.cpp)
add_python_test(dict)

add_python_library(tuple_ext tuple.cpp)
add_python_test(tuple)

add_python_library(str_ext str.cpp)
add_python_test(str)

add_python_library(slice_ext slice.cpp)
add_python_test(slice)

add_python_library(virtual_functions_ext virtual_functions.cpp)
add_python_test(virtual_functions)

add_python_library(back_reference_ext back_reference.cpp)
add_python_test(back_reference)

add_python_library(implicit_ext implicit.cpp)
add_python_test(implicit)

add_python_library(data_members_ext data_members.cpp)
add_python_test(data_members)

add_python_library(ben_scott1_ext ben_scott1.cpp)
add_python_test(ben_scott1)

add_python_library(bienstman1_ext bienstman1.cpp)
add_python_test(bienstman1)

add_python_library(bienstman2_ext bienstman2.cpp)
add_python_test(bienstman2)

add_python_library(bienstman3_ext bienstman3.cpp)
add_python_test(bienstman3)

add_python_library(multi_arg_constructor_ext multi_arg_constructor.cpp)
add_python_test(multi_arg_constructor)

add_python_library(input_iterator input_iterator.cpp)
add_python_library(iterator_ext iterator.cpp)
add_python_test(iterator DEPENDS input_iterator iterator_ext)

add_python_library(stl_iterator_ext stl_iterator.cpp)
add_python_test(stl_iterator)

add_python_library(extract_ext extract.cpp)
add_python_test(extract)

# XXX:
# On Windows the return_opaque_pointer return policy exercised by these
# tests causes an error when loading the Python module:
#
# DLL load failed: A dynamic link library (DLL) initialization routine failed.
#
# This appears to be a pre-existing issue in boost::python. These tests are
# disabled for now.

if (NOT WIN32)
    add_python_library(crossmod_opaque_a crossmod_opaque_a.cpp)
    add_python_library(crossmod_opaque_b crossmod_opaque_b.cpp)
    add_python_test(crossmod_opaque DEPENDS crossmod_opaque_a crossmod_opaque_b)

    add_python_library(opaque_ext opaque.cpp)
    add_python_test(opaque)

    add_python_library(voidptr_ext voidptr.cpp)
    add_python_test(voidptr)
endif()

add_python_library(pickle1_ext pickle1.cpp)
add_python_test(pickle1)

add_python_library(pickle2_ext pickle2.cpp)
add_python_test(pickle2)

add_python_library(pickle3_ext pickle3.cpp)
add_python_test(pickle3)

add_python_library(pickle4_ext pickle4.cpp)
add_python_test(pickle4)

add_python_library(nested_ext nested.cpp)
add_python_test(nested)

add_python_library(docstring_ext docstring.cpp)
add_python_test(docstring)

add_python_library(pytype_function_ext pytype_function.cpp)
add_python_test(pytype_function)

add_python_library(vector_indexing_suite_ext vector_indexing_suite.cpp)
add_python_test(vector_indexing_suite)

add_python_library(pointer_vector_ext pointer_vector.cpp)
add_python_test(pointer_vector)

add_python_library(map_indexing_suite_ext
    map_indexing_suite.cpp
    int_map_indexing_suite.cpp
    a_map_indexing_suite.cpp)
add_python_test(map_indexing_suite)

add_executable(exec exec_.cpp)
target_link_libraries(exec PUBLIC boost-python)
add_cpp_test(exec SCRIPT exec_.py)

add_executable(import_ import_.cpp)
target_link_libraries(import_ PUBLIC boost-python)
add_cpp_test(import_ SCRIPT import_.py)

add_library(indirect_traits_test SHARED indirect_traits_test.cpp)
target_link_libraries(indirect_traits_test PUBLIC boost-python)

add_executable(destroy_test destroy_test.cpp)
target_link_libraries(destroy_test PUBLIC boost-python)
add_cpp_test(destroy_test)

add_executable(pointer_type_id_test pointer_type_id_test.cpp)
target_link_libraries(pointer_type_id_test PUBLIC boost-python)
add_cpp_test(pointer_type_id_test)

add_executable(bases bases.cpp)
target_link_libraries(bases PUBLIC boost-python)
add_cpp_test(bases)

add_executable(if_else if_else.cpp)
target_link_libraries(if_else PUBLIC boost-python)
add_cpp_test(if_else)

add_executable(pointee pointee.cpp)
target_link_libraries(pointee PUBLIC boost-python)
add_cpp_test(pointee)

add_executable(result result.cpp)
target_link_libraries(result PUBLIC boost-python)
add_cpp_test(result)

add_library(string_literal SHARED string_literal.cpp)
target_link_libraries(string_literal PUBLIC boost-python)

add_library(borrowed SHARED borrowed.cpp)
target_link_libraries(borrowed PUBLIC boost-python)

add_library(object_manager SHARED object_manager.cpp)
target_link_libraries(object_manager PUBLIC boost-python)

add_library(copy_ctor_mutates_rhs SHARED copy_ctor_mutates_rhs.cpp)
target_link_libraries(copy_ctor_mutates_rhs PUBLIC boost-python)

add_executable(upcast upcast.cpp)
target_link_libraries(upcast PUBLIC boost-python)
add_cpp_test(upcast)

add_library(select_holder SHARED select_holder.cpp)
target_link_libraries(select_holder PUBLIC boost-python)

add_executable(select_from_python_test select_from_python_test.cpp)
target_link_libraries(select_from_python_test PUBLIC boost-python)
add_cpp_test(select_from_python_test)

add_library(select_arg_to_python_test SHARED select_arg_to_python_test.cpp)
target_link_libraries(select_arg_to_python_test PUBLIC boost-python)

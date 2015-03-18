# This program is free software licenced under MIT Licence. You can
# find a copy of this licence in LICENCE.txt in the top directory of
# source code.
#


#   ai_ab_target_name_for(<output variable> <source file>)
#
# Return the target name associated to a source file. If the path of the
# source file relative from the root of the project is `path/to/source/file.cpp`,
# the target name associated to it is `path.to.source.file`.

function(ai_ab_target_name_for out file)
    file(RELATIVE_PATH _relative ${CMAKE_SOURCE_DIR} ${file})
    string(REGEX REPLACE "\\.cpp$" "" _name ${_relative})
    string(REGEX REPLACE "/" "." _name ${_name})
    set(${out} "${_name}" PARENT_SCOPE)
endfunction()

# Target to build and run all the unit tests
add_custom_target(tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+"
    COMMENT "Build and run all the unit tests.")

# A list of all the test files
file(GLOB_RECURSE AI_AB_TEST_SOURCES "*.cpp")

# Add all the unit tests
foreach(file IN LISTS AI_AB_TEST_SOURCES)
    ai_ab_target_name_for(_target "${file}")
    add_executable(${_target} EXCLUDE_FROM_ALL "${file}")
    target_link_libraries(${_target} c++ c++abi)
    add_test(NAME ${_target} COMMAND ${_target})
    add_dependencies(tests ${_target})
endforeach()

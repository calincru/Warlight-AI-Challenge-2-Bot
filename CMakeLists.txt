# This program is free software licenced under MIT Licence. You can
# find a copy of this licence in LICENCE.txt in the top directory of
# source code.
#


#=============================================================================
# Setup project
#=============================================================================
project(AI_AB CXX)
cmake_minimum_required(VERSION 2.8)

#=============================================================================
# Setup CMake options
#=============================================================================
include(CMakeDependentOption)
option(AI_AB_ENABLE_PEDANTIC "Compile with pedantic enabled." ON)
option(AI_AB_ENABLE_WERROR   "Fail and stop if a warning is triggered." ON)
option(AI_AB_ENABLE_CXX14    "Use the -std=c++14 switch if the compiler supports it." ON)


#=============================================================================
# Setup compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
macro(ai_ab_append_flag lst testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        list(APPEND ${lst} ${flag})
    endif()
endmacro()

ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_W_FLAG                         -W)
ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WALL_FLAG                      -Wall)
ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WEXTRA_FLAG                    -Wextra)
ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WNO_LONG_LONG_FLAG             -Wno-long-long)
ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG -Wno-unused-local-typedefs)
ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WNO_UNUSED_PARAMETER_FLAG      -Wno-unused-parameter)
ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WWRITE_STRINGS_FLAG            -Wwrite-strings)

if (AI_AB_ENABLE_WERROR)
    ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WERROR_FLAG -Werror)
    ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_WX_FLAG     -WX)
endif()

if (AI_AB_ENABLE_PEDANTIC)
    ai_ab_append_flag(AI_AB_CXX_WARNING_FLAGS AI_AB_HAS_PEDANTIC_FLAG -pedantic)
endif()

if (AI_AB_ENABLE_CXX14)
    ai_ab_append_flag(AI_AB_CXX_FEATURE_FLAGS AI_AB_HAS_STDCXX14_FLAG -std=c++14)
endif()


add_compile_options(
    ${AI_AB_CXX_WARNING_FLAGS}
    ${AI_AB_CXX_FEATURE_FLAGS}
)

#=============================================================================
# Setup subdirectories
#=============================================================================
enable_testing()
include_directories(include src)
add_subdirectory(src)
add_subdirectory(test)

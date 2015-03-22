# This program is free software licenced under MIT Licence. You can
# find a copy of this licence in LICENCE.txt in the top directory of
# source code.
#

include_directories(basic-strategies)

# A list of all sources
file(GLOB_RECURSE AI_AB_SOURCES "*.cpp"
                                "basic-strategies/*.cpp")
add_executable(ai_ab ${AI_AB_SOURCES})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_link_libraries(ai_ab c++ c++abi)
endif()

add_custom_target(
    run
    COMMAND ai_ab
    DEPENDS ai_ab
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_subdirectory(basic-strategies)

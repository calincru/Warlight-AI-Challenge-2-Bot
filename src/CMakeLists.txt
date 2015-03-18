# This program is free software licenced under MIT Licence. You can
# find a copy of this licence in LICENCE.txt in the top directory of
# source code.
#


# A list of all sources
file(GLOB_RECURSE AI_AB_SOURCES "*.cpp")
add_executable(ai_ab ${AI_AB_SOURCES})
target_link_libraries(ai_ab c++ c++abi)

add_custom_target(
    run
    COMMAND ai_ab
    DEPENDS ai_ab
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

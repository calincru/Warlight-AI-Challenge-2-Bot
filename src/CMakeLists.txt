# A list of all sources
file(GLOB_RECURSE AI_AB_SOURCES "*.cpp")
add_executable(ai_ab ${AI_AB_SOURCES})

add_custom_target(
    run
    COMMAND ai_ab
    DEPENDS ai_ab
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

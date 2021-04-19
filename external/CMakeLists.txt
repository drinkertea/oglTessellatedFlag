# glfw
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

get_target_property(GLFW_FOLDER glfw FOLDER)
set_target_properties(glfw PROPERTIES FOLDER External/${GLFW_FOLDER})
set_target_properties(uninstall PROPERTIES FOLDER External/${GLFW_FOLDER})

# glfw
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

get_target_property(GLFW_FOLDER glfw FOLDER)
set_target_properties(glfw PROPERTIES FOLDER External/${GLFW_FOLDER})
set_target_properties(uninstall PROPERTIES FOLDER External/${GLFW_FOLDER})

# glad
project(glad)
set(GLAD_DIR "glad")

add_library(glad ${GLAD_DIR}/glad/glad.h ${GLAD_DIR}/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR})
set_target_properties(glad PROPERTIES FOLDER External)

# lodepng
project(lodepng)
set(LP_DIR "lodepng")

add_library(lodepng ${LP_DIR}/lodepng.h ${LP_DIR}/lodepng.cpp)
target_include_directories(lodepng PUBLIC ${LP_DIR})
set_target_properties(lodepng PROPERTIES FOLDER External)

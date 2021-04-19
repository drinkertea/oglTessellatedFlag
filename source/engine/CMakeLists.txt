set(Target Engine)
ADD_LIBRARY(${Target} STATIC)

file(GLOB PublicHeaders "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

target_sources(${Target}
    PUBLIC
        ${PublicHeaders}
    PRIVATE
        window.cpp
        renderer.cpp
)

target_include_directories(${Target}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

add_compile_definitions(VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

target_link_libraries(${Target}
    glfw
    glad
    lodepng
)

set_target_properties(${Target} PROPERTIES FOLDER Core)

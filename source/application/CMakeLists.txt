set(Target oglTessellatedFlag)
add_executable(${Target})

target_sources(${Target}
    PRIVATE
        main.cpp
)

target_link_libraries(${Target}
    Engine
)

set_target_properties(${Target} PROPERTIES FOLDER Applications)
set_target_properties(${Target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_command(TARGET ${Target} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${Target}>/resources)
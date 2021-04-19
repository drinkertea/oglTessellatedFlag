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

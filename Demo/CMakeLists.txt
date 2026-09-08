add_executable(Demo
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

target_link_libraries(Demo
    PRIVATE 
        AudioLib
)
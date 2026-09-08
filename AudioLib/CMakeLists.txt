add_library(AudioLib
        "${CMAKE_CURRENT_SOURCE_DIR}/src/AudioEffect.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/AudioFile.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dr_wav_implementation.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dr_mp3_implementation.cpp"
)

target_include_directories(AudioLib 
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/AudioLib"
        "${PROJECT_SOURCE_DIR}/Dependencies/dr_libs"
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
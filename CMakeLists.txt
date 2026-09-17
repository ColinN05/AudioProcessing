cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AudioProcessing)

add_compile_definitions(TEST_AUDIO_DIR="${PROJECT_SOURCE_DIR}/TestAudioFiles")
add_compile_definitions(OUTPUT_AUDIO_DIR="${PROJECT_SOURCE_DIR}/OutputAudioFiles")

add_subdirectory("${PROJECT_SOURCE_DIR}/AudioLib")
add_subdirectory("${PROJECT_SOURCE_DIR}/Demo")
add_subdirectory("${PROJECT_SOURCE_DIR}/AudioLibDemo")
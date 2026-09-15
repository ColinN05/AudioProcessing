cmake_minimum_required(VERSION 3.20)
project(AudioProcessing)

add_compile_definitions(OUTPUT_AUDIO_DIR="${PROJECT_SOURCE_DIR}/OutputAudioFiles")

add_subdirectory("${PROJECT_SOURCE_DIR}/AudioLib")
add_subdirectory("${PROJECT_SOURCE_DIR}/Demo")
add_subdirectory("${PROJECT_SOURCE_DIR}/AudioLibDemo")
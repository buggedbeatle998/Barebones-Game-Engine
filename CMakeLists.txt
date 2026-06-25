cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME} LANGUAGES C VERSION 0.1.0 DESCRIPTION "Game Thing")

set(CMAKE_C_STANDARD 23)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
file(GLOB SOURCE_FILES
    "src/*.c"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_directories(${PROJECT_NAME} PRIVATE lib)
target_link_libraries(${PROJECT_NAME} m SDL3 X11 pthread dl)

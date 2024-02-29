cmake_minimum_required(VERSION 3.0.0)
project(LearnOpenGL VERSION 0.1.0 LANGUAGES C CXX)

set(DEBUG_BIN_DIR ${CMAKE_BINARY_DIR}/bin/Debug)
set(RELEASE_BIN_DIR ${CMAKE_BINARY_DIR}/bin/Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DEBUG_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${RELEASE_BIN_DIR})

add_subdirectory(3rdparty/glfw)
set(glfw3_DIR ${CMAKE_BUILD_RPATH}/glfw/src)

find_package(glfw3)

include_directories(${GLFW3_INCLUDE_DIR})
include_directories(3rdparty/glm)
include_directories(include)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

aux_source_directory(./src srcs)

add_executable(LearnOpenGL ${srcs})
target_link_libraries(LearnOpenGL glfw)
cmake_minimum_required(VERSION 3.20)
project(zip_iterator_session)

set(CMAKE_CXX_STANDARD 20)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

add_executable(zip_iterator_session main.cpp)

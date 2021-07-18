cmake_minimum_required(VERSION 3.0)

project(BigIntCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(BigIntCpp test.cpp)


target_include_directories(BigIntCpp PUBLIC
                           ${PROJECT_BINARY_DIR}
                          )

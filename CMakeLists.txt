cmake_minimum_required(VERSION 3.15)
project(bitmap)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(bitmap ffa.h bitmap_test.cpp base_allocate.h mfa.h mwfa.h)
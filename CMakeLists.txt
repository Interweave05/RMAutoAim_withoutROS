cmake_minimum_required(VERSION 3.10)
project(RMAutoAim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main main.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(DEBUG)
    message(STATUS "Build type: Debug")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_definitions(NDEBUG)
    message(STATUS "Build type: Release")
endif()

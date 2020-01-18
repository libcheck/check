cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW) # Use <PackageName>_ROOT variables
endif()
if(POLICY CMP0090)
  cmake_policy(SET CMP0090 NEW)
  # export(PACKAGE) does not populate package registry by default.
endif()

project( ProjectUsageTest
    VERSION "0.0.0.0"
    DESCRIPTION "Include Check project in this project in different ways"
    LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(src)


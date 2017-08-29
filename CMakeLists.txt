cmake_minimum_required (VERSION 3.0)
project(flog)
# c++ standerd
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)
# supress excessive configuration
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release")
# require git
find_package(Git REQUIRED)
# include dependencies
include(ExternalProject)
include(${CMAKE_SOURCE_DIR}/script/helper.cmake)
# clone dependencies
git_pull(googletest ${CMAKE_SOURCE_DIR}/dependencies https://github.com/google/googletest)
# compiler flags
if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
endif()
if (CMAKE_GENERATOR MATCHES "Unix Makefiles")
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()
# sub directory
add_subdirectory(flog)
add_subdirectory(test)
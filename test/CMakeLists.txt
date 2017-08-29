cmake_minimum_required (VERSION 3.0)
# target name
set(TEST flog_test)
# c++ standerd
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)
# add include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)
# add link directories
link_directories(${CMAKE_SOURCE_DIR}/dependencies/lib)
# runtime library selection for MSVC
if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
endif()
# add source
file(GLOB_RECURSE SRC_FILES *.cc *.cpp)
file(GLOB_RECURSE HDR_FILES *.h *.hpp)
# executable
add_executable(${TEST} ${SRC_FILES} ${HDR_FILES})
# add dependencies
add_dependencies(${TEST} flog)
add_dependencies(${TEST} googletest)
target_link_libraries(${TEST} flog)
target_link_libraries(${TEST} gtest)
if (NOT MSVC)
    target_link_libraries(${TEST} pthread)
endif()

# platform specific path
get_target_platform(TARGET)

set_target_properties( 
    ${TEST}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET}"
)
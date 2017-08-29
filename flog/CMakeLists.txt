cmake_minimum_required (VERSION 3.0)
# target name
set(LIB_NAME flog)
# c++ standerd
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)
# runtime library selection for MSVC
if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
endif()
# add source
file(GLOB_RECURSE SRC_FILES *.cc *.cpp)
file(GLOB_RECURSE HDR_FILES *.h *.hpp)
# executable
add_library(${LIB_NAME} STATIC ${SRC_FILES} ${HDR_FILES})

# platform specific path
get_target_platform(TARGET)

set_target_properties( 
    ${LIB_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET}"
)
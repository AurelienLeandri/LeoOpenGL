cmake_minimum_required(VERSION 3.0)
project(opengl-engine)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
if (!OPENGL_FOUND)
    MESSAGE("OPENGL NOT FOUND")
endif()
include_directories(${GLEW_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
# Define GLM_INCLUDE_DIRS
if (GLM_FOUND)
    set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(GLM_INCLUDE_DIR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdump-tree-original -std=c++1y -Wall -Wextra -pedantic -g")

include_directories(
        ${CMAKE_SOURCE_DIR}/src
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(src)
add_subdirectory(test)

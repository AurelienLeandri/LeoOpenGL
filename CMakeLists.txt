include("./.env.cmake" RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME LeoOpenGL)

message(STATUS "Using generator ${CMAKE_GENERATOR}")

project(${NAME} VERSION 1.0.0)

MESSAGE("Looking for the opengl32.lib")
find_package(OpenGL REQUIRED)
if (!OPENGL_FOUND)
    MESSAGE("OPENGL NOT FOUND")
endif()

if (NOT DEFINED GLFW_LIB_PATH OR NOT DEFINED GLFW_INCLUDE_PATH)
  message(FATAL_ERROR "GLFW_LIB_PATH and GLFW_INCLUDE_PATH are not set in .env.cmake!")
endif()

message("Using GLFW lib found in ${GLFW_LIB_PATH}")

if (NOT DEFINED ASSIMP_INCLUDE_PATH OR NOT DEFINED ASSIMP_LIB_PATH)
  message(FATAL_ERROR "ASSIMP_LIB_PATH and ASSIMP_INCLUDE_PATH is not set in .env.cmake!")
endif()

message("Using Assimp lib found in ${ASSIMP_LIB_PATH}")

if (NOT DEFINED GLAD_INCLUDE_PATH OR NOT DEFINED GLAD_LIB_PATH)
  message(FATAL_ERROR "GLAD_LIB_PATH and EMBREE_INCLUDE_PATH is not set in .env.cmake!")
endif()

message("Using GLAD lib found in ${EMBREE_LIB_PATH}")

include_directories(${INCLUDE_PATH})
message("Additional include directory: ${INCLUDE_PATH}")

include_directories(src)


file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp SOURCES ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/external/*.cpp ${PROJECT_SOURCE_DIR}/external/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})
 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

set(STBI_NO_DDS)

message(STATUS "CREATING BUILD FOR WINDOWS")

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${GLFW_INCLUDE_PATH}
  ${ASSIMP_INCLUDE_PATH}
  ${GLAD_INCLUDE_PATH}
  ${INCLUDE_PATH}
  )

target_link_directories(${PROJECT_NAME} PUBLIC
  ${GLFW_LIB_PATH}
  ${ASSIMP_LIB_PATH}
  ${GLAD_LIB_PATH}
  ${LIB_PATH}
)

target_link_libraries(${PROJECT_NAME} glfw3 assimp-vc140-mt soil glfw3 opengl32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/external/bin"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)


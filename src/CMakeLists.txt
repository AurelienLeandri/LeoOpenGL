add_subdirectory(material)
add_subdirectory(scene)

set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/opengl-context.cpp
  PARENT_SCOPE)

add_library(engine SHARED ${ENGINE_SOURCES})

target_link_libraries(engine)

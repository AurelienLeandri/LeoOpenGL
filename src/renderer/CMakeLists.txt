add_subdirectory(scene)
add_subdirectory(utils)

set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/engine.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/camera.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/input-manager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/input-manager.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/debug.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/debug.hpp
  PARENT_SCOPE
  )

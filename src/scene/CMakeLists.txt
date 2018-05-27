add_subdirectory(visitor)

set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/node.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/geometry-node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/geometry-node.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mesh.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mesh.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/model.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/model.hpp
  PARENT_SCOPE)

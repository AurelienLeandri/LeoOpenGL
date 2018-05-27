set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/node.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/geometry-node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/geometry-node.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/cube-geometry.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/cube-geometry.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/visitor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/visitor.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/transformation-visitor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/transformation-visitor.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/render-visitor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/render-visitor.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mesh.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mesh.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/model.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/model.hpp
  PARENT_SCOPE)

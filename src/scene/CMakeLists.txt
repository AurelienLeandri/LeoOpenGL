set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/geometry-node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cube-geometry.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/visitor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/transformation-visitor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/render-visitor.cpp
  PARENT_SCOPE)

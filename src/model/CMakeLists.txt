add_subdirectory(components)

set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/registered-object.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/registered-object.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/entity.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/icomponent.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/icomponent.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/component.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/scene-graph.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/scene-graph.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cube-map.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cube-map.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/model-loader.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/model-loader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/type-id.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/component-manager.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/texture-manager.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/entity-manager.hpp
  PARENT_SCOPE
  )


set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/transformation.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/transformation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/volume.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/volume.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/instanced.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/instanced.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/material.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/material.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pbr-material.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pbr-material.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/light.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/light.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibl.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/point-light.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/point-light.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/direction-light.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/direction-light.cpp
  PARENT_SCOPE
  )


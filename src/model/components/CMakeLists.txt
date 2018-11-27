add_subdirectory(types)

set(ENGINE_SOURCES ${ENGINE_SOURCES}
  # ${CMAKE_CURRENT_SOURCE_DIR}/scene-object.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/scene-object.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/transformation.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/transformation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/drawable-collection.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/drawable-collection.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/volume.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/volume.cpp
  PARENT_SCOPE
  )


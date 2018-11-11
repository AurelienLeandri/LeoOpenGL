add_subdirectory(components)

set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/base.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/base.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/registered-object.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/registered-object.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object-register.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object-register.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/component.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
  PARENT_SCOPE
  )


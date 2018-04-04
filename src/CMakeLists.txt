add_subdirectory(material)
add_subdirectory(scene)
add_subdirectory(utils)

set(ENGINE_SOURCES ${ENGINE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/engine.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/camera.hpp
  )

add_library(leo-engine SHARED ${ENGINE_SOURCES})

target_link_libraries(leo-engine ${GLEW_LIBRARY} ${GLM_LIBRARY} -lsfml-graphics -lsfml-window -lsfml-system)

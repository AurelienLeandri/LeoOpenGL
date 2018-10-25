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
  )

add_library(leo-engine SHARED ${ENGINE_SOURCES})

target_link_libraries(
  leo-engine;
  ${GLEW_LIBRARY};
  ${GLM_LIBRARY};
  ${SOIL_LIBRARIES};
  ${ASSIMP_LIBRARIES};
  ${GLFW_LIBRARIES};
  GL;
  X11;
  m;
  dl;
  Xinerama;
  Xrandr;
  Xi;
  Xcursor;
  Xxf86vm;
  pthread
  )

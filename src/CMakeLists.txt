add_subdirectory(model)
add_subdirectory(controller)
add_subdirectory(renderer)

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

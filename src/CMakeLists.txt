add_subdirectory(model)
add_subdirectory(controller)
add_subdirectory(renderer)
add_subdirectory(utils)

add_library(leo-engine SHARED ${ENGINE_SOURCES})

target_link_libraries(
  leo-engine;
  soil;
  assimp;
  glew32;
  glfw3;
  opengl32;
  )

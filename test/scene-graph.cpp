#include <iostream>

#include <renderer/engine.hpp>
#include <renderer/shader.hpp>
#include <renderer/renderer.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/blit-node.hpp>
#include <renderer/main-node.hpp>
#include <renderer/ssao-node.hpp>
#include <renderer/cube-map-node.hpp>
#include <renderer/post-process-node.hpp>
#include <renderer/deferred-lighting-node.hpp>
#include <renderer/gaussian-blur-node.hpp>
#include <renderer/instanced-node.hpp>

#include <model/components/material.hpp>
#include <model/components/pbr-material.hpp>
#include <model/components/volume.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/transformation.hpp>
#include <model/components/instanced.hpp>
#include <model/components/ibl.hpp>
#include <model/entity.hpp>
#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/model-loader.hpp>
#include <model/component-manager.hpp>
#include <model/texture-manager.hpp>
#include <model/entity-manager.hpp>

using namespace leo;

void print_matrix(const glm::mat4x4 &mat)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
      std::cout << mat[i][j] << " ";
    std::cout << std::endl;
  }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void launchRendering(SceneGraph &sceneGraph)
{
  // Engine creating camera, windows and input manager (do Getters)
  Engine engine;
  Camera *camera = &engine.getCamera();
  GLFWwindow &window = engine.getWindow();
  InputManager &inputManager = engine.getInputManager();

  // OpenGLContext(+ SceneData inside)
  OpenGLContext context;
  context.setWindowContext(window, inputManager);

  // Renderer
  Renderer renderer;
  renderer.createSceneContext(context);
  renderer.setSceneGraph(sceneGraph);
  SceneContext &sceneContext = renderer.getSceneContext();

  // Create nodes
  leo::Renderer *graph = &renderer;

  // Shaders
  Shader *shader = graph->createShader("resources/shaders/basic.vs.glsl", "resources/shaders/basic.frag.glsl");
  Shader *gBufferShader = graph->createShader("resources/shaders/basic.vs.glsl", "resources/shaders/gbuffer.frag.glsl");
  Shader *gBufferShaderSSAO = graph->createShader("resources/shaders/basic.vs.glsl", "resources/shaders/gbuffer-view-space.frag.glsl");
  Shader *deferredLightingShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/deferred-lighting.frag.glsl");
  Shader *SSAOShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/ssao.frag.glsl");
  Shader *postProcessShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/reinhard-tone-mapping.frag.glsl");
  Shader *cubeMapShader = graph->createShader("resources/shaders/cube-map.vs.glsl", "resources/shaders/cube-map.frag.glsl");
  Shader *instancingShader = graph->createShader("resources/shaders/instancing.vs.glsl", "resources/shaders/instanced-basic.frag.glsl");
  Shader *gammaCorrectionShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/gamma-correction.frag.glsl");
  Shader *shadowMappingShader = graph->createShader("resources/shaders/dir-shadow-mapping.vs.glsl", "resources/shaders/dir-shadow-mapping.frag.glsl");
  Shader *cubeShadowMapShader = graph->createShader("resources/shaders/point-shadow-mapping.vs.glsl", "resources/shaders/point-shadow-mapping.frag.glsl", "resources/shaders/point-shadow-mapping.geo.glsl");
  Shader *extractCapedBrightnessShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/extract-caped-brightness.frag.glsl");
  Shader *hdrCorrectionShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/hdr-correction.frag.glsl");
  Shader *bloomEffectShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/bloom-effect.frag.glsl");

  // Framebuffers
  Framebuffer *main = graph->createFramebuffer();
  Framebuffer *multisampled = graph->createFramebuffer();
  Framebuffer *postProcess = graph->createFramebuffer();
  Framebuffer *extractCapedBrightnessFB = graph->createFramebuffer();
  Framebuffer *hdrCorrectionFB = graph->createFramebuffer();
  Framebuffer *blurFB = graph->createFramebuffer();
  Framebuffer *ssaoBlurFB = graph->createFramebuffer();
  Framebuffer *bloomEffectFB = graph->createFramebuffer();
  Framebuffer *ssaoFB = graph->createFramebuffer();

  ColorBufferOptions mainBufferOptions;
  mainBufferOptions.pixelFormat = GL_RGB;
  mainBufferOptions.dataFormat = GL_RGB;
  mainBufferOptions.dataType = GL_FLOAT;
  mainBufferOptions.hdr = true;
  main->addColorBuffer(mainBufferOptions);
  main->addColorBuffer(mainBufferOptions);
  main->addColorBuffer();
  main->addColorBuffer();
  main->useRenderBuffer();
  multisampled->addColorBuffer({true});
  multisampled->useRenderBuffer();
  postProcess->addColorBuffer();
  postProcess->useRenderBuffer();
  extractCapedBrightnessFB->addColorBuffer({true});
  extractCapedBrightnessFB->addColorBuffer();
  extractCapedBrightnessFB->useRenderBuffer();
  hdrCorrectionFB->addColorBuffer({true});
  hdrCorrectionFB->useRenderBuffer();
  blurFB->addColorBuffer();
  blurFB->useRenderBuffer();
  ssaoBlurFB->addColorBuffer();
  ssaoBlurFB->useRenderBuffer();
  ssaoFB->addColorBuffer({true});
  ssaoFB->useRenderBuffer();
  bloomEffectFB->addColorBuffer({true});
  bloomEffectFB->useRenderBuffer();

  Framebuffer *gBuffer = graph->createFramebuffer();
  ColorBufferOptions normalPositionBufferOptions;
  normalPositionBufferOptions.pixelFormat = GL_RGB;
  normalPositionBufferOptions.dataFormat = GL_RGB16F;
  normalPositionBufferOptions.dataType = GL_FLOAT;
  gBuffer->addColorBuffer(normalPositionBufferOptions);
  gBuffer->addColorBuffer(normalPositionBufferOptions);
  gBuffer->addColorBuffer();
  gBuffer->addColorBuffer();
  gBuffer->useRenderBuffer();

  RenderNodeOptions options;
  options.clearBufferFlags = 0;
  MainNode *mainNode = graph->createNode<MainNode>(context, sceneContext, sceneGraph, *shader, *camera, options);
  mainNode->setStringId("main node");
  GaussianBlurNode *blurNode = graph->createNode<GaussianBlurNode>(context, sceneContext, sceneGraph);
  blurNode->setStringId("blur node");
  BackgroundNode *cubeMapNode = graph->createNode<BackgroundNode>(context, sceneContext, sceneGraph, *cubeMapShader, *camera);
  cubeMapNode->setStringId("cube map node");
  PostProcessNode *extractCapedBrightnessNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *extractCapedBrightnessShader);
  extractCapedBrightnessNode->setStringId("extract caped node");
  PostProcessNode *hdrCorrectionNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *hdrCorrectionShader);
  hdrCorrectionNode->setStringId("hdr node");
  PostProcessNode *bloomEffectNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *bloomEffectShader);
  bloomEffectNode->setStringId("bloom node");
  PostProcessNode *postProcessNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *postProcessShader);
  postProcessNode->setStringId("post process node");
  PostProcessNode *gammaCorrectionNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *gammaCorrectionShader);
  gammaCorrectionNode->setStringId("gamma correction node");

  mainNode->setFramebuffer(main);
  mainNode->setLabel("Lights");
  cubeMapNode->setFramebuffer(main);
  cubeMapNode->addInNode(*mainNode);
  extractCapedBrightnessNode->addInput(*mainNode, "fb", 0);
  extractCapedBrightnessNode->setFramebuffer(extractCapedBrightnessFB);

  blurNode->addInput(*extractCapedBrightnessNode, "fb", 1);
  blurNode->setFramebuffer(blurFB);

  hdrCorrectionNode->addInput(*extractCapedBrightnessNode, "fb", 0);
  hdrCorrectionNode->setFramebuffer(hdrCorrectionFB);

  bloomEffectNode->addInput(*hdrCorrectionNode, "fb0", 0);
  bloomEffectNode->addInput(*blurNode, "fb1", 0);
  bloomEffectNode->setFramebuffer(bloomEffectFB);

  postProcessNode->addInput(*bloomEffectNode, "fb0", 0);
  postProcessNode->setFramebuffer(postProcess);

  gammaCorrectionNode->addInput(*postProcessNode, "fb", 0);

  // SSAO
  MainNode *gBufferNodeSSAO = graph->createNode<MainNode>(context, sceneContext, sceneGraph, *gBufferShaderSSAO, *camera);
  gBufferNodeSSAO->setStringId("gbuffer ssao node");

  gBufferNodeSSAO->setFramebuffer(gBuffer);
  SSAONode *ssaoNode = graph->createNode<SSAONode>(context, sceneContext, sceneGraph, *SSAOShader, *camera);
  ssaoNode->setStringId("ssao node");

  ssaoNode->addInput(*gBufferNodeSSAO, "fb0", 0);
  ssaoNode->addInput(*gBufferNodeSSAO, "fb1", 1);
  ssaoNode->setFramebuffer(ssaoBlurFB);
  GaussianBlurNode *ssaoBlurNode = graph->createNode<GaussianBlurNode>(context, sceneContext, sceneGraph, 10);
  ssaoBlurNode->setStringId("ssao blur node");

  ssaoBlurNode->addInput(*ssaoNode, "fb0", 0);
  ssaoBlurNode->setFramebuffer(ssaoFB);

  // Lighting
  MainNode *gBufferNode = graph->createNode<MainNode>(context, sceneContext, sceneGraph, *gBufferShader, *camera);
  gBufferNode->setStringId("gbuffer node");

  gBufferNode->setFramebuffer(gBuffer);
  mainNode->addInNode(*gBufferNode);
  DeferredLightingNode *deferredLightingNode = graph->createNode<DeferredLightingNode>(context, sceneContext, sceneGraph, *deferredLightingShader, *camera);
  deferredLightingNode->setStringId("deferred lighting node");

  deferredLightingNode->addInput(*gBufferNode, "fb0", 0);
  deferredLightingNode->addInput(*gBufferNode, "fb1", 1);
  deferredLightingNode->addInput(*gBufferNode, "fb2", 2);
  deferredLightingNode->addInput(*gBufferNode, "fb3", 3);
  deferredLightingNode->addInput(*ssaoBlurNode, "occlusionMap", 0);
  BlitNode *copyDepthNode = graph->createNode<BlitNode>(context, *gBuffer, GL_DEPTH_BUFFER_BIT);
  copyDepthNode->setStringId("copy depth node");

  copyDepthNode->setFramebuffer(main);
  copyDepthNode->addInNode(*deferredLightingNode);
  copyDepthNode->addInNode(*gBufferNode);
  deferredLightingNode->setFramebuffer(main);
  mainNode->addInNode(*copyDepthNode);

  engine.setRenderer(renderer);

  engine.gameLoop();
}

void pbr()
{

  // Scene model
  ComponentManager componentManager;
  TextureManager textureManager;
  EntityManager entityManager;
  ModelLoader modelLoader(entityManager, componentManager, textureManager);

  Entity root;

  SceneGraph sceneGraph;
  CubeMap cubeMap("skybox", "resources/textures/Ice_Lake_Ref.hdr", CubeMapType::EQUIRECTANGULAR);
  sceneGraph.setCubeMap(&cubeMap);
  sceneGraph.setRoot(&root);

  DirectionLight *dl = componentManager.createComponent<DirectionLight>(
      glm::vec3(1.0f, 0.9f, 0.8f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  PointLight *pl = componentManager.createComponent<PointLight>(
      glm::vec3(43.47, 41.31, 40.79),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  Texture *iblTexture = textureManager.createTexture("resources/textures/Ice_Lake_Env.hdr", RGB);
  IBL *ibl = componentManager.createComponent<IBL>(*iblTexture);

  root.addComponent(dl);
  root.addComponent(ibl);

  Entity *pointLight = entityManager.createEntity();
  Transformation *pointLightTr = componentManager.createComponent<Transformation>();
  pointLightTr->setRelativeTranslation(glm::vec3(10, 10, 10));
  pointLight->addComponent(pl);
  pointLight->addComponent(pointLightTr);
  root.addChild(pointLight);

  Volume *cube = componentManager.createComponent<Volume>(Volume::createSphere());

  for (int x = 0; x < 12; x += 2)
  {
    for (int y = 0; y < 12; y += 2)
    {
      Entity *node = entityManager.createEntity();
      PBRMaterial *material = componentManager.createComponent<PBRMaterial>();
      //material->albedo_texture = textureManager.createTexture("resources/textures/rustediron2_basecolor.png", RGBA);
      //material->metalness_texture = textureManager.createTexture("resources/textures/rustediron2_metallic.png", RGBA);
      //material->roughness_texture = textureManager.createTexture("resources/textures/rustediron2_roughness.png", RGBA);
      material->albedo_value = glm::vec3(0.95, 0.30, 0.1);
      material->metalness_value = x / 20.f;
      material->roughness_value = y / 20.f;
      Transformation *transformation = componentManager.createComponent<Transformation>();
      transformation->setRelativeTranslation(glm::vec3(x, y, 0));
      node->addComponent(cube);
      node->addComponent(material);
      node->addComponent(transformation);
      root.addChild(node);
    }
  }

  // Engine creating camera, windows and input manager (do Getters)
  Engine engine;
  Camera *camera = &engine.getCamera();
  GLFWwindow &window = engine.getWindow();
  InputManager &inputManager = engine.getInputManager();

  // OpenGLContext(+ SceneData inside)
  OpenGLContext context;
  context.setWindowContext(window, inputManager);

  // Renderer
  Renderer renderer;
  renderer.createSceneContext(context);
  renderer.setSceneGraph(sceneGraph);
  SceneContext &sceneContext = renderer.getSceneContext();

  // Create nodes
  leo::Renderer *graph = &renderer;

  // Shaders
  Shader *shader = graph->createShader("resources/shaders/basic.vs.glsl", "resources/shaders/basic.frag.glsl");
  Shader *gBufferShader = graph->createShader("resources/shaders/basic.vs.glsl", "resources/shaders/gbuffer-pbr.frag.glsl");
  Shader *deferredLightingShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/deferred-lighting-pbr.frag.glsl");
  Shader *postProcessShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/reinhard-tone-mapping.frag.glsl");
  Shader *cubeMapShader = graph->createShader("resources/shaders/cube-map.vs.glsl", "resources/shaders/cube-map-equirectangular.frag.glsl");
  Shader *instancingShader = graph->createShader("resources/shaders/instancing.vs.glsl", "resources/shaders/instanced-basic.frag.glsl");
  Shader *gammaCorrectionShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/gamma-correction.frag.glsl");
  Shader *shadowMappingShader = graph->createShader("resources/shaders/dir-shadow-mapping.vs.glsl", "resources/shaders/dir-shadow-mapping.frag.glsl");
  Shader *cubeShadowMapShader = graph->createShader("resources/shaders/point-shadow-mapping.vs.glsl", "resources/shaders/point-shadow-mapping.frag.glsl", "resources/shaders/point-shadow-mapping.geo.glsl");
  Shader *extractCapedBrightnessShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/extract-caped-brightness.frag.glsl");
  Shader *hdrCorrectionShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/hdr-correction.frag.glsl");
  Shader *bloomEffectShader = graph->createShader("resources/shaders/post-process.vs.glsl", "resources/shaders/bloom-effect.frag.glsl");

  Framebuffer *postProcess = graph->createFramebuffer();
  postProcess->addColorBuffer();
  postProcess->useRenderBuffer();

  /*()
  // Framebuffers
  Framebuffer *main = graph->createFramebuffer();
  Framebuffer *multisampled = graph->createFramebuffer();
  Framebuffer *postProcess = graph->createFramebuffer();
  Framebuffer *extractCapedBrightnessFB = graph->createFramebuffer();
  Framebuffer *hdrCorrectionFB = graph->createFramebuffer();
  Framebuffer *blurFB = graph->createFramebuffer();
  Framebuffer *bloomEffectFB = graph->createFramebuffer();

  ColorBufferOptions mainBufferOptions;
  mainBufferOptions.pixelFormat = GL_RGB;
  mainBufferOptions.dataFormat = GL_RGB;
  mainBufferOptions.dataType = GL_FLOAT;
  mainBufferOptions.hdr = true;
  main->addColorBuffer(mainBufferOptions);
  main->useRenderBuffer();
  multisampled->addColorBuffer({true});
  multisampled->useRenderBuffer();
  postProcess->addColorBuffer();
  postProcess->useRenderBuffer();
  extractCapedBrightnessFB->addColorBuffer({true});
  extractCapedBrightnessFB->addColorBuffer();
  extractCapedBrightnessFB->useRenderBuffer();
  hdrCorrectionFB->addColorBuffer({true});
  hdrCorrectionFB->useRenderBuffer();
  blurFB->addColorBuffer();
  blurFB->useRenderBuffer();
  bloomEffectFB->addColorBuffer({true});
  bloomEffectFB->useRenderBuffer();
  */

  Framebuffer *main = graph->createFramebuffer();
  ColorBufferOptions mainBufferOptions;
  mainBufferOptions.pixelFormat = GL_RGB;
  mainBufferOptions.dataFormat = GL_RGB;
  mainBufferOptions.dataType = GL_FLOAT;
  mainBufferOptions.hdr = true;
  main->addColorBuffer(mainBufferOptions);
  main->useRenderBuffer();

  Framebuffer *gBuffer = graph->createFramebuffer();
  ColorBufferOptions normalPositionBufferOptions;
  normalPositionBufferOptions.pixelFormat = GL_RGBA;
  normalPositionBufferOptions.dataFormat = GL_RGBA16F;
  normalPositionBufferOptions.dataType = GL_FLOAT;
  gBuffer->addColorBuffer(normalPositionBufferOptions);
  gBuffer->addColorBuffer(normalPositionBufferOptions);
  gBuffer->addColorBuffer();
  gBuffer->addColorBuffer();
  gBuffer->useRenderBuffer();

  PostProcessNode *postProcessNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *postProcessShader);
  postProcessNode->setStringId("post process node");
  PostProcessNode *gammaCorrectionNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *gammaCorrectionShader);
  gammaCorrectionNode->setStringId("gamma correction node");

  /*
  RenderNodeOptions options;
  options.clearBufferFlags = 0;
  MainNode *mainNode = graph->createNode<MainNode>(context, sceneContext, sceneGraph, *shader, *camera, options);
  mainNode->setStringId("main node");
  GaussianBlurNode *blurNode = graph->createNode<GaussianBlurNode>(context, sceneContext, sceneGraph);
  blurNode->setStringId("blur node");
  BackgroundNode *cubeMapNode = graph->createNode<BackgroundNode>(context, sceneContext, sceneGraph, *cubeMapShader, *camera);
  cubeMapNode->setStringId("cube map node");
  PostProcessNode *extractCapedBrightnessNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *extractCapedBrightnessShader);
  extractCapedBrightnessNode->setStringId("extract caped node");
  PostProcessNode *hdrCorrectionNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *hdrCorrectionShader);
  hdrCorrectionNode->setStringId("hdr node");
  PostProcessNode *bloomEffectNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *bloomEffectShader);
  bloomEffectNode->setStringId("bloom node");
  PostProcessNode *postProcessNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *postProcessShader);
  postProcessNode->setStringId("post process node");
  PostProcessNode *gammaCorrectionNode = graph->createNode<PostProcessNode>(context, sceneContext, sceneGraph, *gammaCorrectionShader);
  gammaCorrectionNode->setStringId("gamma correction node");

  mainNode->setFramebuffer(main);
  mainNode->setLabel("Lights");
  cubeMapNode->setFramebuffer(main);
  cubeMapNode->addInNode(*mainNode);
  extractCapedBrightnessNode->addInput(*mainNode, "fb", 0);
  extractCapedBrightnessNode->setFramebuffer(extractCapedBrightnessFB);

  blurNode->addInput(*extractCapedBrightnessNode, "fb", 1);
  blurNode->setFramebuffer(blurFB);

  hdrCorrectionNode->addInput(*extractCapedBrightnessNode, "fb", 0);
  hdrCorrectionNode->setFramebuffer(hdrCorrectionFB);

  bloomEffectNode->addInput(*hdrCorrectionNode, "fb0", 0);
  bloomEffectNode->addInput(*blurNode, "fb1", 0);
  bloomEffectNode->setFramebuffer(bloomEffectFB);

  postProcessNode->addInput(*bloomEffectNode, "fb0", 0);
  postProcessNode->setFramebuffer(postProcess);

  gammaCorrectionNode->addInput(*postProcessNode, "fb", 0);
  */

  postProcessNode->setFramebuffer(postProcess);

  gammaCorrectionNode->addInput(*postProcessNode, "fb", 0);

  // Lighting
  MainNode *gBufferNode = graph->createNode<MainNode>(context, sceneContext, sceneGraph, *gBufferShader, *camera);
  gBufferNode->setStringId("gbuffer node");

  gBufferNode->setFramebuffer(gBuffer);
  //mainNode->addInNode(*gBufferNode);
  DeferredLightingNode *deferredLightingNode = graph->createNode<DeferredLightingNode>(context, sceneContext, sceneGraph, *deferredLightingShader, *camera);
  deferredLightingNode->setStringId("deferred lighting node");

  deferredLightingNode->addInput(*gBufferNode, "fb0", 0);
  deferredLightingNode->addInput(*gBufferNode, "fb1", 1);
  deferredLightingNode->addInput(*gBufferNode, "fb2", 2);
  deferredLightingNode->addInput(*gBufferNode, "fb3", 3);
  deferredLightingNode->setFramebuffer(main);

  BlitNode *copyDepthNode = graph->createNode<BlitNode>(context, *gBuffer, GL_DEPTH_BUFFER_BIT);
  copyDepthNode->setStringId("copy depth node");
  copyDepthNode->setFramebuffer(main);
  copyDepthNode->addInNode(*gBufferNode);
  copyDepthNode->addInNode(*deferredLightingNode);

  BackgroundNode *cubeMapNode = graph->createNode<BackgroundNode>(context, sceneContext, sceneGraph, *cubeMapShader, *camera);
  cubeMapNode->setStringId("cube map node");
  cubeMapNode->setFramebuffer(main);
  cubeMapNode->addInNode(*copyDepthNode);

  postProcessNode->addInput(*deferredLightingNode, "fb0", 0);
  postProcessNode->addInNode(*cubeMapNode);

  engine.setRenderer(renderer);

  engine.gameLoop();
}

void decoupling()
{
  // Scene model
  ComponentManager componentManager;
  TextureManager textureManager;
  EntityManager entityManager;
  ModelLoader modelLoader(entityManager, componentManager, textureManager);

  Entity root;

  Entity *m = modelLoader.loadModel("resources/models/nanosuit/", "nanosuit.obj");
  root.addChild(m);
  SceneGraph sceneGraph;
  CubeMap cubeMap("skybox", "resources/textures");
  sceneGraph.setCubeMap(&cubeMap);
  sceneGraph.setRoot(&root);

  Entity node1;
  root.addChild(&node1);

  Material *material = componentManager.createComponent<Material>();

  Volume *cube = componentManager.createComponent<Volume>(Volume::createCube(1.f));

  node1.addComponent(cube);
  material->diffuse_texture = textureManager.createTexture("resources/textures/wood.png", RGBA);
  material->specular_texture = textureManager.createTexture("resources/textures/wood.png", RGBA);
  material->normal_map = textureManager.createTexture("resources/textures/toy_box_normal.png", RGBA);
  material->parallax_map = textureManager.createTexture("resources/textures/toy_box_disp.png", RGBA);
  material->shininess = 32.f;

  Material *material2 = componentManager.createComponent<Material>();
  material2->diffuse_texture = textureManager.createTexture("resources/textures/bricks2.jpg", RGB);
  material2->specular_texture = textureManager.createTexture("resources/textures/bricks2.jpg", RGB);
  material2->normal_map = textureManager.createTexture("resources/textures/bricks2_normal.jpg", RGB);
  material2->parallax_map = textureManager.createTexture("resources/textures/bricks2_disp.jpg", RGB);
  material2->shininess = 32.f;

  Material *lightMaterial = componentManager.createComponent<Material>();
  lightMaterial->emissive_value = glm::vec3(1.0, 1.0, 1.0);

  node1.addComponent(material2);

  PointLight *pl = componentManager.createComponent<PointLight>(
      glm::vec3(9.2f, 9.2f, 9.2f),
      glm::vec3(9.6f, 9.6f, 9.6f),
      glm::vec3(9.6f, 9.6f, 9.6f));

  Transformation *t2 = componentManager.createComponent<Transformation>();

  t2->setRelativeTranslation(glm::vec3(0.f, 2.f, 0.f));
  t2->setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2->setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  Entity node2;
  node2.addComponent(t2);
  node2.addComponent(cube);
  node2.addComponent(pl);
  node2.addComponent(lightMaterial);

  Transformation *t3 = componentManager.createComponent<Transformation>();

  DirectionLight *dl = componentManager.createComponent<DirectionLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  t3->setRelativeTranslation(glm::vec3(4.f, 2.f, 0.f));
  t3->setRelativeRotation(glm::vec3(45.f, 0.f, 0.f));
  t3->setRelativeScaling(glm::vec3(0.5f, 0.5f, 0.5f));
  Entity node3;
  node3.addComponent(dl);

  Entity node31;
  node3.addChild(&node31);
  node31.addComponent(material);
  node31.addComponent(cube);
  node31.addComponent(t3);

  Material *groundMaterial = componentManager.createComponent<Material>();
  groundMaterial->diffuse_texture = textureManager.createTexture("resources/textures/brickwall.jpg", SRGB);
  groundMaterial->specular_texture = textureManager.createTexture("resources/textures/brickwall.jpg", SRGB);
  groundMaterial->normal_map = textureManager.createTexture("resources/textures/brickwall_normal.jpg", RGB);
  groundMaterial->shininess = 32.f;

  Material *wallMat = componentManager.createComponent<Material>();
  wallMat->diffuse_texture = textureManager.createTexture("resources/textures/wood.png", SRGBA);
  wallMat->specular_texture = wallMat->diffuse_texture;
  wallMat->shininess = 1.0f;

  Entity node4;
  root.addChild(&node4);
  Volume *ground = componentManager.createComponent<Volume>(Volume::createPlane(10.f, 10.f));
  node4.addComponent(groundMaterial);

  Entity node40;
  node4.addChild(&node40);
  node40.addComponent(ground);
  node40.addComponent(wallMat);

  Entity node41;
  Transformation *t41 = componentManager.createComponent<Transformation>();
  t41->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t41->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 0.f));
  t41->setRelativeTranslation(glm::vec3(0.f, 10.f, -5.f));
  node4.addChild(&node41);
  node41.addComponent(ground);
  node41.addComponent(t41);

  Entity node42;
  Transformation *t42 = componentManager.createComponent<Transformation>();
  t42->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t42->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 3.14f));
  t42->setRelativeTranslation(glm::vec3(0.f, 10.f, 5.f));
  node4.addChild(&node42);
  node42.addComponent(ground);
  node42.addComponent(t42);

  Entity node43;
  Transformation *t43 = componentManager.createComponent<Transformation>();
  t43->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t43->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 3.14f / 2.f));
  t43->setRelativeTranslation(glm::vec3(5.f, 10.f, 0.f));
  node4.addChild(&node43);
  node43.addComponent(ground);
  node43.addComponent(t43);

  Entity node44;
  Transformation *t44 = componentManager.createComponent<Transformation>();
  t44->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t44->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, -3.14f / 2.f));
  t44->setRelativeTranslation(glm::vec3(-5.f, 10.f, 0.f));
  node4.addChild(&node44);
  node44.addComponent(ground);
  node44.addComponent(t44);

  Entity node45;
  Transformation *t45 = componentManager.createComponent<Transformation>();
  t45->setRelativeRotation(glm::vec3(0.f, 0.f, 3.14f));
  t45->setRelativeTranslation(glm::vec3(0.f, 20.f, 0.f));
  node4.addChild(&node45);
  node45.addComponent(ground);
  node45.addComponent(t45);

  node1.addChild(&node3);
  node1.addChild(&node2);

  launchRendering(sceneGraph);
}

int main()
{
  //decoupling();
  pbr();
  //testInstanced();
  return 0;
}

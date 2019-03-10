#include <renderer/main-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <model/components/transformation.hpp>
#include <renderer/camera.hpp>
#include <sstream>

namespace leo
{

MainNode::MainNode(Shader &shader, const Camera &camera)
    : RenderNode(shader, camera)
{
    { // Lights
        glGenBuffers(1, &this->_lightsUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
        auto size = sizeof(PointLightUniform);
        glBufferData(GL_UNIFORM_BUFFER,
                     MAX_NUM_LIGHTS * (sizeof(PointLightUniform) + sizeof(DirectionLightUniform)),
                     NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void MainNode::render()
{
    this->_load();
    this->_unload();
}

void MainNode::_load()
{
    // Define the viewport dimensions
    glClearColor(0.07, 0.07, 0.07, 1);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    this->_loadShader();
}

void MainNode::_loadShader()
{
    this->_shader.use();
    this->_shader.setMat4("view", this->_camera.getViewMatrix());
    this->_shader.setMat4("projection", glm::perspective(this->_camera.getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));

    this->_loadInputFramebuffers();
    this->_loadOutputFramebuffer();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    this->_loadLightsToShader();

    this->_setModelMatrix();
}

void MainNode::_loadInputFramebuffers()
{
    int inputNumber = 0;
    for (auto &p : this->_inputs)
    {
        Framebuffer *input = p.second;
        const std::vector<TextureWrapper> &cb = input->getColorBuffers();
        for (GLuint i = 0; i < cb.size(); i++)
        {
            std::stringstream number;
            number << inputNumber;
            glUniform1i(glGetUniformLocation(this->_shader.getProgram(), (p.first + number.str()).c_str()), inputNumber);
            glActiveTexture(GL_TEXTURE0 + inputNumber);
            glBindTexture(GL_TEXTURE_2D, cb[i].getId());
            inputNumber++;
        }
    }
    this->_materialTextureOffset = inputNumber;
}

void MainNode::_loadOutputFramebuffer()
{
    if (this->_output)
    {
        this->_output->loadFrameBuffer();
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void MainNode::_loadLightsToShader()
{
  unsigned int ubiLights = glGetUniformBlockIndex(this->_shader.getProgram(), "s1");
  if (ubiLights != GL_INVALID_INDEX)
  {
    glUniformBlockBinding(this->_shader.getProgram(), ubiLights, 1);
  }

  glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->_lightsUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
  int i = 0;
  for (auto &p : this->_pointLights)
  {
    PointLightUniform &plu = p.second;
    glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLightUniform), sizeof(PointLightUniform), &plu);
    i++;
  }
  i = 0;
  unsigned int offset = MAX_NUM_LIGHTS * sizeof(PointLightUniform);
  for (auto &p : this->_directionLights)
  {
    DirectionLightUniform &dlu = p.second;
    glBufferSubData(GL_UNIFORM_BUFFER, offset + i * sizeof(DirectionLightUniform), sizeof(DirectionLightUniform), &dlu);
    i++;
  }
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MainNode::_setModelMatrix(const Transformation *transformation)
{
  this->_shader.setMat4("model", transformation->getTransformationMatrix());
}

void MainNode::_setModelMatrix()
{
  glm::mat4 m;
  this->_shader.setMat4("model", m);
}

void MainNode::_unload()
{
}

} // namespace leo
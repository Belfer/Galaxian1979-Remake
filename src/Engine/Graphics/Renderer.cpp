#include "Renderer.hpp"
#include <glad/glad.h>

using namespace NHTV;

Renderer::Renderer() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH_CLAMP);
  glEnable(GL_MULTISAMPLE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

Renderer::~Renderer() {}

size_t Renderer::newCamera() {
  static size_t counter = 0;
  m_cameraMap[counter] = Camera();
  return counter++;
}

size_t Renderer::newTexture() {
  static size_t counter = 0;
  m_textureMap[counter] = Texture();
  return counter++;
}

size_t Renderer::newShader() {
  static size_t counter = 0;
  m_shaderMap[counter] = Shader();
  return counter++;
}

size_t Renderer::newMesh() {
  static size_t counter = 0;
  m_meshMap[counter] = Mesh();
  return counter++;
}

Camera &Renderer::getCamera(size_t id) {
  // TODO assert
  return m_cameraMap[id];
}

Texture &Renderer::getTexture(size_t id) {
  // TODO assert
  return m_textureMap[id];
}

Shader &Renderer::getShader(size_t id) {
  // TODO assert
  return m_shaderMap[id];
}

Mesh &Renderer::getMesh(size_t id) {
  // TODO assert
  return m_meshMap[id];
}

void Renderer::removeCamera(size_t id) {
  // TODO
}

void Renderer::removeTexture(size_t id) {
  // TODO
}

void Renderer::removeShader(size_t id) {
  // TODO
}

void Renderer::removeMesh(size_t id) {
  // TODO
}

void Renderer::bindCamera(size_t id) {
  // TODO assert
  m_renderState.camera = id;
}

void Renderer::bindTexture(size_t id) {
  // TODO assert
  m_renderState.texture = id;
}

void Renderer::bindShader(size_t id) {
  // TODO assert
  m_renderState.shader = id;
}

void Renderer::draw(IDrawable *pDrawable) {
  const auto &camera = getCamera(m_renderState.camera);
  auto &texture = getTexture(m_renderState.texture);
  auto &shader = getShader(m_renderState.shader);
  pDrawable->draw(camera, texture, shader);
}

bool Renderer::createFramebuffer(int width, int height) {
  // Generate the framebuffer
  glGenFramebuffers(1, &m_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

  // Generate the texture to render to
  glGenTextures(1, &m_renderTarget);
  glBindTexture(GL_TEXTURE_2D, m_renderTarget);

  // Sumbit an empty image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, 0);

  // Use nearest filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Set render target as our colour attachement num 0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTarget, 0);

  // Set the list of draw buffers
  glDrawBuffers(1, m_drawBuffers);

  // Check if framebuffer is ok
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Unable to setup framebuffer!" << std::endl;
    return false;
  }

  // Generate the fullscreen quad's VAO
  glGenVertexArrays(1, &m_screenQuadVAO);
  glBindVertexArray(m_screenQuadVAO);

  // Generate the fullscreen quads's VBO
  glGenBuffers(1, &m_screenQuadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVBO);
  static const GLfloat screenQuadData[] = {
      -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadData), screenQuadData,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::bindFramebuffer(int width, int height) {
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  glBindTexture(GL_TEXTURE_2D, m_renderTarget);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::drawFramebuffer(int width, int height) {
  // Render to the screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // Render on the whole framebuffer, complete from the lower left corner to
  // the upper right
  glViewport(0, 0, width, height);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use screen quad shader
  m_postFxShader.bind();

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_renderTarget);

  //    m_postFxShader.setInt("renderTarget", 0);
  //    m_postFxShader.setFloat("time", m_fxTime);
  //    m_postFxShader.setFloat("bloomK", m_fxBloomK);
  //    m_postFxShader.setVec2("resolution", glm::vec2(1.f / width, 1.f /
  //    height));
  //    m_postFxShader.setVec4("waveParams", m_fxWaveParams);
  //    m_postFxShader.setVec4("color", m_fxColor);
  //    m_postFxShader.setVec4("position", m_fxPosition);
  //    m_postFxShader.setVec4("offset", m_fxOffset);

  // Draw the triangles !
  glBindVertexArray(m_screenQuadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
  glBindVertexArray(0);
}

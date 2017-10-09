#include "Renderer.hpp"
#include <glad/glad.h>

using namespace NHTV;

size_t Renderer::loadTexture(const std::string &filename) {
  //
}

size_t Renderer::genTexture() {
  //
}

void Renderer::unloadTexture(size_t texture) {
  //
}

void Renderer::bindTexture(size_t texture) {
  //
}

size_t Renderer::loadShader(const std::string &filename) {
  //
}

size_t Renderer::genShader(const std::string &vert, const std::string &frag) {
  //
}

void Renderer::unloadShader(size_t shader) {
  //
}

void Renderer::bindShader(size_t shader) {
  //
}

size_t Renderer::loadMesh(const std::string &filename) {
  //
}

size_t Renderer::genMesh(size_t vertCount, size_t vertSize) {
  uint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertCount * vertSize, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  static size_t meshCounter = -1;
  meshCounter++;

  m_meshMap[meshCounter] = VBO;
  return meshCounter;
}

void Renderer::unloadMesh(size_t mesh) {
  //
}

void Renderer::drawMesh(size_t mesh) {
  //
}

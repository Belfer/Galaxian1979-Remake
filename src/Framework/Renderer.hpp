#pragma once

#include "NonCopyable.hpp"
#include "SpriteBatch.hpp"
#include <glm/glm.hpp>
#include <map>
#include <string>

using namespace glm;

namespace NHTV {

class Renderer : public NonCopyable {
public:
  static Renderer &Instance() {
    static Renderer *pInstance = nullptr;
    if (pInstance == nullptr)
      pInstance = new Renderer();
    return *pInstance;
  }

  size_t loadTexture(const std::string &filename);
  size_t genTexture();
  void unloadTexture(size_t texture);
  void bindTexture(size_t texture);

  size_t loadShader(const std::string &filename);
  size_t genShader(const std::string &vert, const std::string &frag);
  void unloadShader(size_t shader);
  void bindShader(size_t shader);

  size_t loadMesh(const std::string &filename);
  size_t genMesh(size_t vertCount, size_t vertSize);
  void unloadMesh(size_t mesh);
  void drawMesh(size_t mesh);

private:
  Renderer();

  std::map<size_t, uint> m_textureMap;
  std::map<size_t, uint> m_shaderMap;
  std::map<size_t, uint> m_meshMap;
};
}

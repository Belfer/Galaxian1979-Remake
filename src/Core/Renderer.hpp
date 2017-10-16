#pragma once

#include "Mesh.hpp"
#include "NonCopyable.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <map>
#include <string>

using namespace glm;

namespace NHTV {

class Renderer : public NonCopyable {
public:
  Renderer() {}
  ~Renderer() {}
  //  static Renderer &Instance() {
  //    static Renderer *pInstance = nullptr;
  //    if (pInstance == nullptr)
  //      pInstance = new Renderer();
  //    return *pInstance;
  //  }

private:
  std::map<size_t, Texture> m_textureMap;
  std::map<size_t, Shader> m_shaderMap;
  std::map<size_t, Mesh> m_meshMap;
};
}

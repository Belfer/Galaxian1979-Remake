#pragma once

#include <glad/glad.h>

#include "Camera.hpp"
#include "Drawable.hpp"
#include "Material.hpp"
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
  Renderer();
  ~Renderer();

  size_t newCamera();

  size_t newTexture();

  size_t newShader();

  size_t newMesh();

  Camera &getCamera(size_t id);

  Texture &getTexture(size_t id);

  Shader &getShader(size_t id);

  Mesh &getMesh(size_t id);

  void removeCamera(size_t id);

  void removeTexture(size_t id);

  void removeShader(size_t id);

  void removeMesh(size_t id);

  void bindCamera(size_t id);

  void bindTexture(size_t id);

  void bindShader(size_t id);

  void draw(IDrawable *pDrawable);

  // void draw(const Camera &camera, const Mesh &mesh, const Material
  // &material);

  void setPostFxShader(const std::string &vertfile,
                       const std::string &fragfile);

  Shader &getPostFxShader();

private:
  struct RenderState {
    size_t camera;
    size_t texture;
    size_t shader;
  };

  friend class Engine;
  bool createFramebuffer(int width, int height);

  void bindFramebuffer(int width, int height);

  void drawFramebuffer(int width, int height);

  Shader m_postFxShader;

  GLenum m_drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  GLuint m_screenQuadVAO = 0;
  GLuint m_screenQuadVBO = 0;

  GLuint m_frameBuffer = 0;
  GLuint m_renderTarget = 0;

  Camera m_mainCamera;
  RenderState m_renderState;

  std::map<size_t, Camera> m_cameraMap;
  std::map<size_t, Texture> m_textureMap;
  std::map<size_t, Shader> m_shaderMap;
  std::map<size_t, Mesh> m_meshMap;
};

inline void Renderer::setPostFxShader(const std::string &vertfile,
                                      const std::string &fragfile) {
  m_postFxShader.load(vertfile, fragfile);
}

inline Shader &Renderer::getPostFxShader() { return m_postFxShader; }
}

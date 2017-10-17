#pragma once

#include "Mesh.hpp"
#include "NonCopyable.hpp"
#include "Renderer.hpp"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

namespace NHTV {
class SpriteBatch : public NonCopyable {
public:
  struct Quad {
    Vertex v1;
    Vertex v2;
    Vertex v3;
    Vertex v4;
  };

  SpriteBatch(Renderer &renderer, size_t size = 100);

  void drawSprite(const vec4 &rct, const vec4 &uvs, const vec4 &color);

  void drawSprite(const vec4 &rct, const vec4 &uvs, const vec4 &color,
                  const mat4x4 &trx);

  void configure();
  void update();
  void draw(const Camera &camera, const Material &material);
  void clear();

private:
  Renderer &m_renderer;
  size_t m_meshId;
  Mesh *m_meshPtr;

  std::vector<Quad> m_quads;
};
}

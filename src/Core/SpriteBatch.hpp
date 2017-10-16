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
  void draw();
  void clear();

private:
  Renderer &m_renderer;
  Mesh m_mesh;
  std::vector<Quad> m_quads;
};

inline void SpriteBatch::update() { m_mesh.update(); }

inline void SpriteBatch::draw() { m_mesh.draw(); }

inline void SpriteBatch::clear() {
  m_mesh.clear();
  m_quads.clear();
}
}

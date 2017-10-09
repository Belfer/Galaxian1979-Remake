#pragma once

#include "NonCopyable.hpp"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

namespace NHTV {

class Renderer;
class SpriteBatch : public NonCopyable {
public:
  struct Vertex {
    vec4 position;
    vec4 color;
    vec2 texCoord;
  };

  struct Quad {
    Vertex v1;
    Vertex v2;
    Vertex v3;
    Vertex v4;
  };

  SpriteBatch(Renderer &renderer, size_t size = 100);

  void drawSprite(const vec4 &rct, const vec4 &uvs, const vec4 &color, float z);

  void drawSprite(const vec4 &rct, const vec4 &uvs, const vec4 &color, float z,
                  const mat4x4 &trx);

  void update();
  void draw();

private:
  Renderer &m_renderer;

  bool m_updated;
  std::vector<Quad> m_quads;
};
}

#pragma once

#include "NonCopyable.hpp"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

namespace NHTV {

class Renderer;
class LineBatch : public NonCopyable {
public:
  struct Vertex {
    vec4 position;
    vec4 color;
  };

  struct Line {
    Vertex v1;
    Vertex v2;
  };

  LineBatch(Renderer &renderer, size_t size = 100);

  void drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                const vec4 &colB);

  void drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                const vec4 &colB, const mat4x4 &trx);

  void update();
  void draw();

private:
  Renderer &m_renderer;

  bool m_updated;
  std::vector<Line> m_lines;
};
}

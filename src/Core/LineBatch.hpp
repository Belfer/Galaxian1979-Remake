#pragma once

#include "Mesh.hpp"
#include "NonCopyable.hpp"
#include "Renderer.hpp"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

namespace NHTV {
class LineBatch : public NonCopyable {
public:
  struct Line {
    Vertex v1;
    Vertex v2;
  };

  LineBatch(Renderer &renderer, size_t size = 100);

  void drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                const vec4 &colB);

  void drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                const vec4 &colB, const mat4x4 &trx);

  void configure();
  void update();
  void draw();
  void clear();

private:
  Renderer &m_renderer;
  Mesh m_mesh;
  std::vector<Line> m_lines;
};

inline void LineBatch::update() { m_mesh.update(); }

inline void LineBatch::draw() { m_mesh.draw(); }

inline void LineBatch::clear() {
  m_mesh.clear();
  m_lines.clear();
}
}

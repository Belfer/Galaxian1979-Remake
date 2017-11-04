#pragma once

#include "Mesh.hpp"
#include "Engine/Core/NonCopyable.hpp"
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
  ~LineBatch();

  void drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                const vec4 &colB);

  void drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                const vec4 &colB, const mat4x4 &trx);

  void configure();
  void update();
  void draw(const Camera &camera, const Material &material);
  void clear();

private:
  Renderer &m_renderer;
  size_t m_meshId;
  Mesh *m_meshPtr;

  // std::vector<Line> m_lines;
};
}

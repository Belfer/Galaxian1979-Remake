#pragma once

#include "Mesh.hpp"
#include "NonCopyable.hpp"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

namespace NHTV {

class Renderer;
class LineBatch : public NonCopyable {
public:
  struct Vertex : public IVertex {
    Vertex();
    vec4 position;
    vec4 color;

    virtual void data(void *d) const override;
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
  Mesh m_mesh;
};

LineBatch::Vertex::Vertex() {}

inline void LineBatch::Vertex::data(void *d) const {
  float *fd = static_cast<float *>(d);
  fd[0] = position.x;
  fd[1] = position.y;
  fd[2] = position.z;
  fd[3] = position.w;
  fd[4] = color.x;
  fd[5] = color.y;
  fd[6] = color.z;
  fd[7] = color.w;
}
}

#pragma once

#include "Mesh.hpp"
#include "NonCopyable.hpp"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

namespace NHTV {

class Renderer;
class SpriteBatch : public NonCopyable {
public:
  struct Vertex : public IVertex {
    Vertex();
    vec4 position;
    vec4 color;
    vec2 texCoord;

    virtual void data(void *d) const override;
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
  Mesh m_mesh;
};

SpriteBatch::Vertex::Vertex() {
  // VertexAlign::Desc desc;
  // desc.size = desc._4;
  // desc.type = desc.FLOAT;
  // align.attribs.emplace_back(desc);
}

inline void SpriteBatch::Vertex::data(void *d) const {
  float *fd = static_cast<float *>(d);
  fd[0] = position.x;
  fd[1] = position.y;
  fd[2] = position.z;
  fd[3] = position.w;
  fd[4] = color.x;
  fd[5] = color.y;
  fd[6] = color.z;
  fd[7] = color.w;
  fd[8] = texCoord.x;
  fd[9] = texCoord.y;
}
}

#pragma once

#include <glad/glad.h>

#include "NonCopyable.hpp"
#include "Types.hpp"
#include <assert.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace NHTV {
struct Vertex {
  glm::vec4 position{0};
  glm::vec4 color{0};
  glm::vec2 texcoord{0};
};

class Mesh : public NonCopyable {
public:
  enum MeshMode {
    STATIC = GL_STATIC_DRAW,
    DYNAMIC = GL_DYNAMIC_DRAW,
    STREAM = GL_STREAM_DRAW
  };

  enum DrawMode {
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    TRIANGLES = GL_TRIANGLES
  };

  struct Params {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    MeshMode meshMode = STATIC;
    DrawMode drawMode = TRIANGLES;
  };

public:
  void configure(const Params &params);

  void load(const std::string &filename);

  void draw();

  void clear();

  void update();

  uint addVertex(const Vertex &vertex);

  void addTriangle(uint a, uint b, uint c);

private:
  void generate();

  uint m_mesh = 0;
  Params m_params;

  bool m_updated = false;
};

inline uint Mesh::addVertex(const Vertex &vertex) {
  m_updated = true;
  m_params.vertices.emplace_back(vertex);
  return m_params.vertices.size() - 1;
}

inline void Mesh::addTriangle(uint a, uint b, uint c) {
  m_updated = true;
  m_params.indices.emplace_back(a);
  m_params.indices.emplace_back(b);
  m_params.indices.emplace_back(c);
}
}
#pragma once

#include <glad/glad.h>

#include "Engine/Core/NonCopyable.hpp"
#include "Engine/Core/Types.hpp"
#include "Vertex.hpp"
#include <assert.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace NHTV {
class Mesh {
public:
  enum MeshType { PRIMITIVE = 0, INDEXED = 1 };

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
    MeshType meshType = INDEXED;
    MeshMode meshMode = STATIC;
    DrawMode drawMode = TRIANGLES;
  };

public:
  void configure(const Params &params);

  void load(const std::string &filename);

  void draw();

  void clear();

  void update(bool resize);

  uint addVertex(const Vertex &vertex);

  void addTriangle(uint a, uint b, uint c);

private:
  void generate(bool resize);

  uint m_mesh = 0;
  uint m_VBO = 0;
  uint m_EBO = 0;
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

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
class Batch {
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
  Batch(size_t vertCount = 400, size_t indxCount = 600)
      : m_vertCount(vertCount), m_indxCount(indxCount) {
    resize(vertCount, indxCount);
  }

  ~Batch() {}

  void resize(size_t vertCount, size_t indxCount);

  void draw();

  void clear();

  void setMeshType(MeshType meshType);

  void setMeshMode(MeshMode meshMode);

  void setDrawMode(DrawMode drawMode);

  uint addVertex(const Vertex &vertex);

  void addTriangle(uint a, uint b, uint c);

private:
  size_t m_vertCount;
  size_t m_indxCount;

  uint m_VBO = 0;
  uint m_EBO = 0;
  Params m_params;
};

inline void Batch::setMeshType(MeshType meshType) {
  m_params.meshType = meshType;
}

inline void Batch::setMeshMode(MeshMode meshMode) {
  m_params.meshMode = meshMode;
}

inline void Batch::setDrawMode(DrawMode drawMode) {
  m_params.drawMode = drawMode;
}

inline uint Batch::addVertex(const Vertex &vertex) {
  m_params.vertices.emplace_back(vertex);
  return m_params.vertices.size() - 1;
}

inline void Batch::addTriangle(uint a, uint b, uint c) {
  m_params.indices.emplace_back(a);
  m_params.indices.emplace_back(b);
  m_params.indices.emplace_back(c);
}
}

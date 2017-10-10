#include "Mesh.hpp"
#include <glad/glad.h>

using namespace NHTV;

template <typename Vertex>
void Mesh<Vertex>::configure(const MeshParams<Vertex> &params) {
  const size_t allocSize = params.vertCount * sizeof(Vertex);

  glGenBuffers(1, &m_mesh);
  glBindBuffer(GL_ARRAY_BUFFER, m_mesh);
  glBufferData(GL_ARRAY_BUFFER, allocSize, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_updated = true;
}

template <typename Vertex> void Mesh<Vertex>::generate() {
  //
  m_updated = false;
}

template <typename Vertex>
void Mesh<Vertex>::load(const std::string &filename) {
  //
}

template <typename Vertex> void Mesh<Vertex>::unload() {
  //
}

template <typename Vertex> void Mesh<Vertex>::draw() {
  //
}

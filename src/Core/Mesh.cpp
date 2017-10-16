#include "Mesh.hpp"

using namespace NHTV;

void Mesh::configure(const Params &params) {
  m_params = params;
  generate();
}

void Mesh::generate() {
  float buffer[m_params.vertices.size() * 10];
  uint idx = 0;
  for (auto vert : m_params.vertices) {
    buffer[idx++] = vert.position.x;
    buffer[idx++] = vert.position.y;
    buffer[idx++] = vert.position.z;
    buffer[idx++] = vert.position.w;
    buffer[idx++] = vert.color.x;
    buffer[idx++] = vert.color.y;
    buffer[idx++] = vert.color.z;
    buffer[idx++] = vert.color.w;
    buffer[idx++] = vert.texcoord.x;
    buffer[idx++] = vert.texcoord.y;
  }

  glGenVertexArrays(1, &m_mesh);
  glBindVertexArray(m_mesh);

  // Vertex buffer
  uint VBO = 0;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, m_params.meshMode);

  // Position
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                        (void *)(0));
  glEnableVertexAttribArray(0);

  // Color
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                        (void *)(4 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texcoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                        (void *)(8 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Element buffer
  uint EBO = 0;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_params.indices.size() * sizeof(uint),
               &m_params.indices[0], m_params.meshMode);

  glBindVertexArray(0);
}

void Mesh::load(const std::string &filename) {
  // TODO
  generate();
}

void Mesh::update() {
  if (m_updated) {
    generate();
  }
  m_updated = false;
}

void Mesh::draw() {
  glBindVertexArray(m_mesh);
  const size_t indxCount = m_params.indices.size();
  glDrawElements(m_params.drawMode, indxCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::clear() {
  m_params.vertices.clear();
  m_params.indices.clear();
  glDeleteVertexArrays(1, &m_mesh);
}

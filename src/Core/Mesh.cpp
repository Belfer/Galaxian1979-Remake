#include "Mesh.hpp"
#include <array>

using namespace NHTV;

void Mesh::configure(const Params &params) {
  m_params = params;
  generate();
}

void Mesh::generate() {
  const size_t vBufferSize = m_params.vertices.size() * 10;
  float vBuffer[vBufferSize];
  uint idx = 0;
  for (auto vert : m_params.vertices) {
    vBuffer[idx++] = vert.position.x;
    vBuffer[idx++] = vert.position.y;
    vBuffer[idx++] = vert.position.z;
    vBuffer[idx++] = vert.position.w;
    vBuffer[idx++] = vert.color.x;
    vBuffer[idx++] = vert.color.y;
    vBuffer[idx++] = vert.color.z;
    vBuffer[idx++] = vert.color.w;
    vBuffer[idx++] = vert.texcoord.x;
    vBuffer[idx++] = vert.texcoord.y;
  }

  glGenVertexArrays(1, &m_mesh);
  glBindVertexArray(m_mesh);

  // Vertex buffer
  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vBufferSize, vBuffer, m_params.meshMode);

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
  const size_t eBufferSize = m_params.indices.size() * sizeof(uint);
  uint *eBuffer = &m_params.indices[0];

  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, eBufferSize, eBuffer,
               m_params.meshMode);

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
  assert(!m_updated && "Update mesh before drawing!");

  glBindVertexArray(m_mesh);

  if (m_params.meshType == PRIMITIVE) {
    glDrawArrays(m_params.drawMode, 0, m_params.vertices.size());

  } else if (m_params.meshType == INDEXED) {
    const size_t indxCount = m_params.indices.size();
    glDrawElements(m_params.drawMode, indxCount, GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}

void Mesh::clear() {
  // Clear buffers
  glBindVertexArray(m_mesh);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glDeleteBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glDeleteBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDeleteVertexArrays(1, &m_mesh);
  glBindVertexArray(0);

  // Re-configure
  configure(Params());
}

#include "Batch.hpp"
#include <array>

using namespace NHTV;

void Batch::resize(size_t vertCount, size_t indxCount) {
  // Vertex buffer
  float *vBuffer = new float[vertCount]{0};
  const size_t vBufferSize = vertCount * sizeof(float);

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
  uint *eBuffer = new uint[indxCount]{0};
  const size_t eBufferSize = indxCount * sizeof(uint);

  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, eBufferSize, eBuffer,
               m_params.meshMode);

  delete vBuffer;
  delete eBuffer;
  vBuffer = nullptr;
  eBuffer = nullptr;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Batch::draw() {
  float *vBuffer = new float[m_vertCount];
  uint *eBuffer = new uint[m_indxCount];

  const size_t vBufferSize = m_vertCount * sizeof(float);
  const size_t eBufferSize = m_indxCount * sizeof(uint);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  size_t vi = 0, vc = 0;
  size_t ei = 0, ec = 0;
  while (vi < m_params.vertices.size() && ei < m_params.indices.size()) {
    vc = 0;
    for (size_t i = 0; i < m_vertCount; ++i) {
      if (vi + i < m_params.vertices.size()) {
        const Vertex &vert = m_params.vertices[vi + i];
        vBuffer[vc++] = vert.position.x;
        vBuffer[vc++] = vert.position.y;
        vBuffer[vc++] = vert.position.z;
        vBuffer[vc++] = vert.position.w;
        vBuffer[vc++] = vert.color.x;
        vBuffer[vc++] = vert.color.y;
        vBuffer[vc++] = vert.color.z;
        vBuffer[vc++] = vert.color.w;
        vBuffer[vc++] = vert.texcoord.x;
        vBuffer[vc++] = vert.texcoord.y;
      } else {
      }
    }
    for (size_t i = 0; i < m_indxCount; ++i) {
      if (ei + i < m_params.indices.size()) {
        eBuffer[i] = m_params.indices[ei + i];
      } else {
        eBuffer[i] = 0;
      }
    }

    vi += m_vertCount;
    ei += m_indxCount;

    glBufferSubData(GL_ARRAY_BUFFER, 0, vBufferSize, vBuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, eBufferSize, eBuffer);

    if (m_params.meshType == PRIMITIVE) {

      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glDrawArrays(m_params.drawMode, 0, m_params.vertices.size());
      glBindBuffer(GL_ARRAY_BUFFER, 0);

    } else if (m_params.meshType == INDEXED) {
      const size_t indxCount = m_params.indices.size();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
      glDrawElements(m_params.drawMode, indxCount, GL_UNSIGNED_INT, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  }

  clear();
}

void Batch::clear() {
  m_params.vertices.clear();
  m_params.indices.clear();
}

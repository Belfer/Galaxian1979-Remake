#include "Mesh.hpp"
#include <iostream>

using namespace NHTV;

template <typename Num>
void bufferData(const IAttribArray *pAttribArr, const IVertex::Desc &desc,
                size_t idx, size_t offset) {
  const AttribArray<Num> &attribArray =
      *static_cast<const AttribArray<Num> *>(pAttribArr);

  for (uint i = 0; i < attribArray.data.size(); ++i) {
    std::cout << attribArray.data[i] << "\n";
  }

  glBindBuffer(GL_ARRAY_BUFFER, idx);
  glBufferData(GL_ARRAY_BUFFER, attribArray.data.size(), &attribArray.data[0],
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(idx, desc.size, desc.type, desc.norm,
                        desc.size * sizeof(Num), (void *)offset);
}

void Mesh::configure(const Params &params) {
  m_params = params;
  generate();
}

void Mesh::generate() {
  glGenVertexArrays(1, &m_mesh);
  glBindVertexArray(m_mesh);

  glGenBuffers(m_params.attributes.size(), &m_mesh);
  size_t offset = 0;
  for (size_t i = 0; i < m_params.attributes.size(); ++i) {
    auto desc = m_params.attributes[i].pAttribArr->desc;

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    // GL_STATIC_DRAW);

    if (desc.type == IVertex::Desc::BYTE) {
      bufferData<char>(m_params.attributes[i].pAttribArr, desc, i, offset);
      offset += desc.size * sizeof(char);

    } else if (desc.type == IVertex::Desc::INT) {
      bufferData<int>(m_params.attributes[i].pAttribArr, desc, i, offset);
      offset += desc.size * sizeof(int);

    } else if (desc.type == IVertex::Desc::FLOAT) {
      bufferData<float>(m_params.attributes[i].pAttribArr, desc, i, offset);
      offset += desc.size * sizeof(float);
    }

    glEnableVertexAttribArray(i);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  m_updated = false;
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
  //  for (size_t i = 0; i < m_params.attributes.size(); ++i) {
  //    const AttribArray<float> &attribArray =
  //        *static_cast<const AttribArray<float> *>(
  //            m_params.attributes[i].pAttribArr);
  //    std::cout << "";
  //  }

  glBindVertexArray(m_mesh);
  // glDrawArrays(GL_TRIANGLES, 0, m_vertCount);
  glBindVertexArray(0);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Mesh::clear() {
  m_params.attributes.clear();
  m_params.indices.clear();
  glDeleteBuffers(1, &m_mesh);
}

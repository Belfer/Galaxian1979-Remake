#pragma once

#include <glad/glad.h>

#include "NonCopyable.hpp"
#include "Types.hpp"
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

namespace NHTV {
struct IVertex {
  virtual void data(void *d) const = 0;

  /**
   * @brief Descriptor struct
   */
  struct Desc {
    enum S { _1 = 1, _2 = 2, _3 = 3, _4 = 4 };
    enum T { BYTE = GL_BYTE, INT = GL_INT, FLOAT = GL_FLOAT };

    Desc() {}
    Desc(S size, T type, bool norm) {}

    S size = _1;
    T type = FLOAT;
    bool norm = false;
  };

protected:
  friend class Mesh;
  size_t totalSize;
  std::vector<Desc> descList;
};

struct IAttribArray {
  IVertex::Desc desc;
};
template <typename T> struct AttribArray : public IAttribArray {
  std::vector<T> data;
};

struct AttribHandle {
  IAttribArray *pAttribArr;
};

class Mesh : public NonCopyable {
public:
  enum MeshMode {
    STATIC = GL_STATIC_DRAW,
    DYNAMIC = GL_DYNAMIC_DRAW,
    STREAM = GL_STREAM_DRAW
  };

  struct Params {
    size_t vertSize;
    std::vector<AttribHandle> attributes;
    std::vector<uint> indices;
    MeshMode meshMode = STATIC;
  };

public:
  void configure(const Params &params);

  void load(const std::string &filename);

  void draw();

  void clear();

  void update();

  template <typename Vertex> uint addVertex(const Vertex &vertex);

  void addTriangle(uint a, uint b, uint c);

private:
  void generate();

  template <typename Num>
  void addAttribute(void *data, const IVertex::Desc &desc,
                    AttribHandle &attribHandle);

  uint m_mesh;
  Params m_params;

  size_t m_vertCount = -1;
  bool m_updated = false;
};

template <typename Num>
void Mesh::addAttribute(void *data, const IVertex::Desc &desc,
                        AttribHandle &attribHandle) {
  const Num d = *reinterpret_cast<Num *>(data);
  auto attrib = static_cast<AttribArray<Num> *>(attribHandle.pAttribArr);
  attrib->desc = desc;
  attrib->data.emplace_back(d);
}

template <typename Vertex> inline uint Mesh::addVertex(const Vertex &vertex) {
  static_assert(std::is_base_of<IVertex, Vertex>::value,
                "Vertex doesn't inherit from IVertex!");

  if (m_params.attributes.empty()) {
    m_params.vertSize = vertex.totalSize;
    m_params.attributes.resize(vertex.descList.size());
    for (size_t i = 0; i < vertex.descList.size(); ++i) {
      auto desc = vertex.descList[i];
      auto &attribHandle = m_params.attributes[i];

      if (desc.type == IVertex::Desc::BYTE) {
        attribHandle.pAttribArr = new AttribArray<char>();

      } else if (desc.type == IVertex::Desc::INT) {
        attribHandle.pAttribArr = new AttribArray<int>();

      } else if (desc.type == IVertex::Desc::FLOAT) {
        attribHandle.pAttribArr = new AttribArray<float>();
      }
    }
  }

  // Make sure the Vertex type is compatible.
  assert(m_params.vertSize == vertex.totalSize && "Vertex type invalid!");
  assert(m_params.attributes.size() == vertex.descList.size() && "Vertex type invalid!");

  char data[vertex.totalSize];
  vertex.data(data);

  size_t idx = 0;
  for (size_t i = 0; i < m_params.attributes.size(); ++i) {
    auto desc = vertex.descList[i];

    if (desc.type == IVertex::Desc::BYTE) {
      addAttribute<char>(&data[idx], desc, m_params.attributes[i]);
      idx += sizeof(char);

    } else if (desc.type == IVertex::Desc::INT) {
      addAttribute<int>(&data[idx], desc, m_params.attributes[i]);
      idx += sizeof(int);

    } else if (desc.type == IVertex::Desc::FLOAT) {
      addAttribute<float>(&data[idx], desc, m_params.attributes[i]);
      idx += sizeof(float);
    }
  }

  m_updated = true;
  return ++m_vertCount;
}

inline void Mesh::addTriangle(uint a, uint b, uint c) {
  m_params.indices.emplace_back(a);
  m_params.indices.emplace_back(b);
  m_params.indices.emplace_back(c);
  m_updated = true;
}
}

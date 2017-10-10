#pragma once

#include "Types.hpp"
#include <string>
#include <vector>

namespace NHTV {
template <typename Vertex> struct MeshParams {
  std::vector<Vertex> vertices;
  std::vector<uint> indices;
};

struct IMesh {};
template <typename Vertex> class Mesh : public IMesh {
public:
  void configure(const MeshParams<Vertex> &params);
  void generate();

  void load(const std::string &filename);
  void unload();

  void draw();

  inline uint addVertex(const Vertex &vert) {
    m_updated = true;
    m_params.vertices.emplace_back(vert);
    return m_params.vertices.size() - 1;
  }

  inline void addTriangle(uint a, uint b, uint c) {
    m_updated = true;
    m_params.indices.emplace_back(a);
    m_params.indices.emplace_back(b);
    m_params.indices.emplace_back(c);
  }

private:
  uint m_mesh;
  MeshParams<Vertex> m_params;

  bool m_updated = false;
};
}

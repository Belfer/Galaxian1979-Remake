#include "LineBatch.hpp"

using namespace NHTV;

void genLine(LineBatch::Line &line, const vec3 &a, const vec3 &b,
             const vec4 &colA, const vec4 &colB);

void applyTrx(LineBatch::Line &line, const mat4x4 &trx);

void addLineToMesh(Mesh &mesh, const LineBatch::Line &line);

LineBatch::LineBatch(Renderer &renderer, size_t size) : m_renderer(renderer) {
  Mesh::Params params;
  m_mesh.configure(params);
}

void LineBatch::configure() {
  Mesh::Params params;
  params.meshMode = Mesh::DYNAMIC;
  m_mesh.configure(params);
}

void LineBatch::drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                         const vec4 &colB) {
  Line line;
  genLine(line, a, b, colA, colB);
  addLineToMesh(m_mesh, line);
}

void LineBatch::drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                         const vec4 &colB, const mat4x4 &trx) {
  Line line;
  genLine(line, a, b, colA, colB);
  applyTrx(line, trx);
  addLineToMesh(m_mesh, line);
}

void genLine(LineBatch::Line &line, const vec3 &a, const vec3 &b,
             const vec4 &colA, const vec4 &colB) {
  line.v1.position.x = a.x;
  line.v1.position.y = a.y;
  line.v1.position.z = a.z;
  line.v1.position.w = 1;
  line.v1.color = colA;

  line.v2.position.x = b.x;
  line.v2.position.y = b.y;
  line.v2.position.z = b.z;
  line.v2.position.w = 1;
  line.v2.color = colB;
}

void applyTrx(LineBatch::Line &line, const mat4x4 &trx) {
  line.v1.position = trx * line.v1.position;
  line.v2.position = trx * line.v2.position;
}

void addLineToMesh(Mesh &mesh, const LineBatch::Line &line) {
  mesh.addVertex(line.v1);
  mesh.addVertex(line.v2);
}

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

void LineBatch::update() {
  //
}

void LineBatch::draw() {
  //
}

void genLine(LineBatch::Line &line, const vec3 &a, const vec3 &b,
             const vec4 &colA, const vec4 &colB) {
  //
}

void applyTrx(LineBatch::Line &line, const mat4x4 &trx) {
  line.v1.position = trx * line.v1.position;
  line.v2.position = trx * line.v2.position;
}

void addLineToMesh(Mesh &mesh, const LineBatch::Line &line) {
  mesh.addVertex<LineBatch::Vertex>(line.v1);
  mesh.addVertex<LineBatch::Vertex>(line.v2);
}

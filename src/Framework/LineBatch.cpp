#include "LineBatch.hpp"

using namespace NHTV;

void genLine(LineBatch::Line &line, const vec3 &a, const vec3 &b,
             const vec4 &colA, const vec4 &colB);

void applyTrx(LineBatch::Vertex &v, const mat4x4 &trx);

LineBatch::LineBatch(Renderer &renderer, size_t size) : m_renderer(renderer) {
  m_lines.reserve(size);
  m_updated = true;
}

void LineBatch::drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                         const vec4 &colB) {
  m_updated = true;

  Line line;
  genLine(line, a, b, colA, colB);
  m_lines.emplace_back(line);
}

void LineBatch::drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                         const vec4 &colB, const mat4x4 &trx) {
  m_updated = true;

  Line line;
  genLine(line, a, b, colA, colB);
  applyTrx(line.v1, trx);
  applyTrx(line.v2, trx);
  m_lines.emplace_back(line);
}

void LineBatch::update() {
  if (m_updated) {
    m_updated = false;
  }
}

void LineBatch::draw() {
  //
}

void genLine(LineBatch::Line &line, const vec3 &a, const vec3 &b,
             const vec4 &colA, const vec4 &colB) {
  //
}

void applyTrx(LineBatch::Vertex &v, const mat4x4 &trx) {
  v.position = trx * v.position;
}

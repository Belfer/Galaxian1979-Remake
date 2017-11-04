#include "LineBatch.hpp"

using namespace NHTV;

void genLine(LineBatch::Line &line, const vec3 &a, const vec3 &b,
             const vec4 &colA, const vec4 &colB);

void applyTrx(LineBatch::Line &line, const mat4x4 &trx);

void addLineToMesh(Mesh &mesh, const LineBatch::Line &line);

LineBatch::LineBatch(Renderer &renderer, size_t size) : m_renderer(renderer) {
  // m_lines.reserve(size);
  m_meshId = renderer.newMesh();
  m_meshPtr = &renderer.getMesh(m_meshId);
}

LineBatch::~LineBatch() {
  m_renderer.removeMesh(m_meshId);
  m_meshPtr = nullptr;
}

void LineBatch::configure() {
  Mesh::Params params;
  params.meshType = Mesh::PRIMITIVE;
  params.meshMode = Mesh::DYNAMIC;
  params.drawMode = Mesh::LINES;
  m_meshPtr->configure(params);
}

void LineBatch::update() { m_meshPtr->update(false); }

void LineBatch::draw(const Camera &camera, const Material &material) {
  glDisable(GL_DEPTH_TEST);

  Shader &shader = m_renderer.getShader(material.shader);

  shader.setMat4x4("Projection", camera.getProjection());
  shader.setMat4x4("ModelView", camera.getView());
  shader.bind();
  m_meshPtr->draw();
}

void LineBatch::clear() {
  m_meshPtr->clear();
  // m_lines.clear();
}

void LineBatch::drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                         const vec4 &colB) {
  Line line;
  genLine(line, a, b, colA, colB);
  addLineToMesh(*m_meshPtr, line);
}

void LineBatch::drawLine(const vec3 &a, const vec3 &b, const vec4 &colA,
                         const vec4 &colB, const mat4x4 &trx) {
  Line line;
  genLine(line, a, b, colA, colB);
  applyTrx(line, trx);
  addLineToMesh(*m_meshPtr, line);
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

#include "SpriteBatch.hpp"

using namespace NHTV;

void genQuad(SpriteBatch::Quad &quad, const vec4 &rct, const vec4 &uvs,
             const vec4 &color);

void applyTrx(SpriteBatch::Quad &quad, const mat4x4 &trx);

void addQuadToMesh(Mesh &mesh, const SpriteBatch::Quad &quad);

SpriteBatch::SpriteBatch(Renderer &renderer, size_t size)
    : m_renderer(renderer) {
  m_quads.reserve(size);
  m_meshId = renderer.newMesh();
  m_meshPtr = &renderer.getMesh(m_meshId);
}

void SpriteBatch::configure() {
  Mesh::Params params;
  params.meshMode = Mesh::DYNAMIC;
  m_meshPtr->configure(params);
}

void SpriteBatch::update() { m_meshPtr->update(); }

void SpriteBatch::draw(const Camera &camera, const Material &material) {
  glDisable(GL_DEPTH_TEST);

  Texture &texture = m_renderer.getTexture(material.texture);
  Shader &shader = m_renderer.getShader(material.shader);

  shader.setMat4x4("Projection", camera.getProjection());
  shader.setMat4x4("ModelView", camera.getView());
  shader.bind();
  texture.bind();
  m_meshPtr->draw();
}

void SpriteBatch::clear() {
  m_meshPtr->clear();
  m_quads.clear();
}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color) {
  Quad quad;
  genQuad(quad, rct, uvs, color);
  addQuadToMesh(*m_meshPtr, quad);
  m_quads.emplace_back(quad);
}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color, const mat4x4 &trx) {
  Quad quad;
  genQuad(quad, rct, uvs, color);
  applyTrx(quad, trx);
  addQuadToMesh(*m_meshPtr, quad);
  m_quads.emplace_back(quad);
}

void genQuad(SpriteBatch::Quad &quad, const vec4 &rct, const vec4 &uvs,
             const vec4 &color) {
  quad.v1.texcoord.x = uvs.x;
  quad.v1.texcoord.y = uvs.y;
  quad.v1.position.x = rct.x;
  quad.v1.position.y = rct.y;
  quad.v1.position.z = 1;
  quad.v1.position.w = 1;
  quad.v1.color = color;

  quad.v2.texcoord.x = uvs.x;
  quad.v2.texcoord.y = uvs.w;
  quad.v2.position.x = rct.x;
  quad.v2.position.y = rct.w;
  quad.v2.position.z = 1;
  quad.v2.position.w = 1;
  quad.v2.color = color;

  quad.v3.texcoord.x = uvs.z;
  quad.v3.texcoord.y = uvs.y;
  quad.v3.position.x = rct.z;
  quad.v3.position.y = rct.y;
  quad.v3.position.z = 1;
  quad.v3.position.w = 1;
  quad.v3.color = color;

  quad.v4.texcoord.x = uvs.z;
  quad.v4.texcoord.y = uvs.w;
  quad.v4.position.x = rct.z;
  quad.v4.position.y = rct.w;
  quad.v4.position.z = 1;
  quad.v4.position.w = 1;
  quad.v4.color = color;
}

void applyTrx(SpriteBatch::Quad &quad, const mat4x4 &trx) {
  quad.v1.position = trx * quad.v1.position;
  quad.v2.position = trx * quad.v2.position;
  quad.v3.position = trx * quad.v3.position;
  quad.v4.position = trx * quad.v4.position;
}

void addQuadToMesh(Mesh &mesh, const SpriteBatch::Quad &quad) {
  const uint i1 = mesh.addVertex(quad.v1);
  const uint i2 = mesh.addVertex(quad.v2);
  const uint i3 = mesh.addVertex(quad.v3);
  const uint i4 = mesh.addVertex(quad.v4);
  mesh.addTriangle(i1, i3, i2);
  mesh.addTriangle(i2, i3, i4);
}

#include "SpriteBatch.hpp"

using namespace NHTV;

void genQuad(SpriteBatch::Quad &quad, const vec4 &rct, const vec4 &uvs,
             const vec4 &color);

void applyTrx(SpriteBatch::Quad &quad, const mat4x4 &trx);

void addQuadToBatch(Batch &batch, const SpriteBatch::Quad &quad);

SpriteBatch::SpriteBatch(Renderer &renderer, size_t size)
    : m_renderer(renderer) {
  m_quads.reserve(size);
  m_batch.resize(size * 4, size * 6);
}

SpriteBatch::~SpriteBatch() {
}

void SpriteBatch::configure() {
  m_batch.setMeshType(Batch::INDEXED);
  m_batch.setMeshMode(Batch::DYNAMIC);
  m_batch.setDrawMode(Batch::TRIANGLES);
}

void SpriteBatch::draw(const Camera &camera, Texture &texture, Shader &shader) {
  glDisable(GL_DEPTH_TEST);

  shader.setMat4x4("Projection", camera.getProjection());
  shader.setMat4x4("ModelView", camera.getView());
  shader.bind();
  texture.bind();

  size_t idx = 0;
  while (idx < m_quads.size()) {
    for (size_t i = idx; i < idx + 100 && i < m_quads.size(); ++i) {
      addQuadToBatch(m_batch, m_quads[i]);
    }
    idx += 100;

    m_batch.draw();
  }
  m_quads.clear();

}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color) {
  Quad quad;
  genQuad(quad, rct, uvs, color);
  m_quads.emplace_back(quad);
}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color, const mat4x4 &trx) {
  Quad quad;
  genQuad(quad, rct, uvs, color);
  applyTrx(quad, trx);
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

void addQuadToBatch(Batch &batch, const SpriteBatch::Quad &quad) {
  const uint i1 = batch.addVertex(quad.v1);
  const uint i2 = batch.addVertex(quad.v2);
  const uint i3 = batch.addVertex(quad.v3);
  const uint i4 = batch.addVertex(quad.v4);
  batch.addTriangle(i1, i3, i2);
  batch.addTriangle(i2, i3, i4);
}

#include "SpriteBatch.hpp"

using namespace NHTV;

void genQuad(SpriteBatch::Quad &quad, const vec4 &rct, const vec4 &uvs,
             const vec4 &color, float z);

void applyTrx(SpriteBatch::Vertex &v, const mat4x4 &trx);

SpriteBatch::SpriteBatch(Renderer &renderer, size_t size)
    : m_renderer(renderer) {
  m_quads.reserve(size);
  m_updated = true;
}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color, float z) {
  m_updated = true;

  Quad quad;
  genQuad(quad, rct, uvs, color, z);
  m_quads.emplace_back(quad);
}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color, float z, const mat4x4 &trx) {
  m_updated = true;

  Quad quad;
  genQuad(quad, rct, uvs, color, z);
  applyTrx(quad.v1, trx);
  applyTrx(quad.v2, trx);
  applyTrx(quad.v3, trx);
  applyTrx(quad.v4, trx);
  m_quads.emplace_back(quad);
}

void SpriteBatch::update() {
  if (m_updated) {
    m_updated = false;
  }
}

void SpriteBatch::draw() {
  //
}

void genQuad(SpriteBatch::Quad &quad, const vec4 &rct, const vec4 &uvs,
             const vec4 &color, float z) {
  quad.v1.texCoord.x = uvs.x;
  quad.v1.texCoord.y = uvs.y;
  quad.v1.position.x = rct.x;
  quad.v1.position.y = rct.y;
  quad.v1.position.z = z;
  quad.v1.color = color;

  quad.v2.texCoord.x = uvs.x;
  quad.v2.texCoord.y = uvs.w;
  quad.v2.position.x = rct.x;
  quad.v2.position.y = rct.w;
  quad.v2.position.z = z;
  quad.v2.color = color;

  quad.v3.texCoord.x = uvs.z;
  quad.v3.texCoord.y = uvs.y;
  quad.v3.position.x = rct.z;
  quad.v3.position.y = rct.y;
  quad.v3.position.z = z;
  quad.v3.color = color;

  quad.v4.texCoord.x = uvs.z;
  quad.v4.texCoord.y = uvs.w;
  quad.v4.position.x = rct.z;
  quad.v4.position.y = rct.w;
  quad.v4.position.z = z;
  quad.v4.color = color;
}

void applyTrx(SpriteBatch::Vertex &v, const mat4x4 &trx) {
  v.position = trx * v.position;
}

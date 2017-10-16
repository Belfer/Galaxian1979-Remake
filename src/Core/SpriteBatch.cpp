#include "SpriteBatch.hpp"

using namespace NHTV;

void genQuad(SpriteBatch::Quad &quad, const vec4 &rct, const vec4 &uvs,
             const vec4 &color, float z);

void applyTrx(SpriteBatch::Quad &quad, const mat4x4 &trx);

void addQuadToMesh(Mesh &mesh, const SpriteBatch::Quad &quad);

SpriteBatch::SpriteBatch(Renderer &renderer, size_t size)
    : m_renderer(renderer) {
  Mesh::Params params;
  m_mesh.configure(params);
}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color, float z) {
  Quad quad;
  genQuad(quad, rct, uvs, color, z);
  addQuadToMesh(m_mesh, quad);
}

void SpriteBatch::drawSprite(const vec4 &rct, const vec4 &uvs,
                             const vec4 &color, float z, const mat4x4 &trx) {
  Quad quad;
  genQuad(quad, rct, uvs, color, z);
  applyTrx(quad, trx);
  addQuadToMesh(m_mesh, quad);
}

void SpriteBatch::update() {
  //
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

void applyTrx(SpriteBatch::Quad &quad, const mat4x4 &trx) {
  quad.v1.position = trx * quad.v1.position;
  quad.v2.position = trx * quad.v2.position;
  quad.v3.position = trx * quad.v3.position;
  quad.v4.position = trx * quad.v4.position;
}

void addQuadToMesh(Mesh &mesh, const SpriteBatch::Quad &quad) {
  const uint i1 = mesh.addVertex<SpriteBatch::Vertex>(quad.v1);
  const uint i2 = mesh.addVertex<SpriteBatch::Vertex>(quad.v2);
  const uint i3 = mesh.addVertex<SpriteBatch::Vertex>(quad.v3);
  const uint i4 = mesh.addVertex<SpriteBatch::Vertex>(quad.v4);
  mesh.addTriangle(i1, i2, i3);
  mesh.addTriangle(i3, i2, i4);
}

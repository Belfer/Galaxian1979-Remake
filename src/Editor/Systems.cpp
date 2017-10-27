#include "Systems.hpp"
#include "Game/Components.hpp"

/**
 * @brief PhysicsSystem
 */
void PhysicsSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, PhysicsCmp>(
      [dt](Entity e, TransformCmp &trx, PhysicsCmp &phs) {
        // phs.acc = phs.force / phs.mass;
        // phs.vel += phs.acc;
        trx.pos += phs.vel * (float)dt;
        trx.rot *= angleAxis(phs.trq * (float)dt, vec3(0, 0, 1));
      });
}

/**
 * @brief SpriteSystem
 */
void SpriteSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  mat4x4 modelMat;
  es.each<TransformCmp, SpriteCmp>(
      [=, &modelMat](Entity e, TransformCmp &trx, SpriteCmp &spr) {
        modelMat = translate(trx.pos) * toMat4(trx.rot) * scale(trx.scl);
        // pSpriteBatch->drawSprite(spriteRct, spriteUVs, ent.col, model);
      });
}

/*#include "Game/Factory.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/projection.hpp>

void DebugSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, PhysicsCmp, ColliderCmp>(
      [=](Entity e, TransformCmp &trx, PhysicsCmp &phs, ColliderCmp &col) {
        // Draws velocity vector (blue)
        m_app.DrawLine(trx.pos.x, trx.pos.y, trx.pos.x + phs.vel.x * dt,
                       trx.pos.y + phs.vel.y * dt, 0xFF0000FF, 0xFF0000FF);

        // Draws approx of circle collider with two diagonals (green)
        m_app.DrawLine(trx.pos.x - col.radius, trx.pos.y - col.radius,
                       trx.pos.x + col.radius, trx.pos.y + col.radius,
                       0x00FF00FF, 0x00FF00FF);
        m_app.DrawLine(trx.pos.x + col.radius, trx.pos.y - col.radius,
                       trx.pos.x - col.radius, trx.pos.y + col.radius,
                       0x00FF00FF, 0x00FF00FF);
      });

  int screenWidth;
  int screenHeight;
  m_app.GetScreenSize(screenWidth, screenHeight);

  // Draw grid
  float w = screenWidth / 32.f;
  float h = screenHeight / 32.f;
  const uint gridCol = 0x202020FF;
  for (uint i = 0; i < 32; ++i) {
    m_app.DrawLine(i * w, 0, i * w, screenHeight, gridCol);

    for (uint j = 0; j < 32; ++j) {
      m_app.DrawLine(0, j * h, screenWidth, j * h, gridCol);
    }
  }

  // m_app.SetFont("../resources/fonts/invaders.fnt");
  // m_app.DrawString(("FPS: " + m_fpsStr).c_str(), 5, 20, 0.5f);
  // m_app.SetFont(nullptr);
}*/

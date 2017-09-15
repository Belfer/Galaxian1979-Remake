#include "Factory.hpp"

namespace Factory {

Entity newParticle(EntityManager &entityMgr, const vec2 &pos,
                   ParticleCmp::Mode mode, float maxLife,
                   ParticleCmp::ResetFn resetFn, ParticleCmp::UpdateFn updateFn,
                   size_t size) {
  auto e = entityMgr.create();
  e.assign<TransformCmp>(pos, vec2(1, 1), 0);
  e.assign<ParticleCmp>(mode, maxLife, resetFn, updateFn, size);
  return e;
}

Entity newBullet(EntityManager &entityMgr, const vec2 &pos, const vec2 &vel,
                 unsigned int sprite, unsigned int color, float collider,
                 float damage) {
  auto e = entityMgr.create();
  e.assign<TransformCmp>(pos, vec2(1, 1), (atan2(vel.y, vel.x) * RAD2DEG) + 90);
  e.assign<PhysicsCmp>(vel, vec2(0, 0), 1, vec2(0, 0));
  e.assign<SpriteCmp>(sprite, color);
  e.assign<ColliderCmp>(collider);
  e.assign<BulletCmp>(damage);
  return e;
}

Entity newPlayer(EntityManager &entityMgr, const vec2 &pos, unsigned int sprite,
                 float collider, unsigned int bulletSpr, float fireRate,
                 float speed, unsigned char lives) {
  auto e = entityMgr.create();
  e.assign<TransformCmp>(pos, vec2(1, 1), 0);
  e.assign<PhysicsCmp>(vec2(0, 0), vec2(0, 0), 1, vec2(0, 0));
  e.assign<SpriteCmp>(sprite, 0xFFFFFFFF);
  e.assign<ColliderCmp>(collider);
  e.assign<HealthCmp>(1, 0, 1);
  e.assign<PlayerCmp>(bulletSpr, fireRate, speed, lives);
  return e;
}

Entity newEnemy(EntityManager &entityMgr, unsigned int sprite, float collider,
                unsigned int bulletSpr, float fireRate, float speed,
                unsigned int points, unsigned int color, const vec2 &idxPos) {
  auto e = entityMgr.create();
  e.assign<TransformCmp>(vec2(0, 0), vec2(1, 1), 0);
  e.assign<PhysicsCmp>(vec2(0, 0), vec2(0, 0), 1, vec2(0, 0));
  e.assign<SpriteCmp>(sprite, color);
  e.assign<ColliderCmp>(collider);
  e.assign<HealthCmp>(1, 0, 1);
  e.assign<EnemyCmp>(bulletSpr, fireRate, speed, points, idxPos);
  return e;
}
}

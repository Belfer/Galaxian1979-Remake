#include "Factory.hpp"
#include "Core/Global.hpp"

namespace GVars {

float playerSpeed = 300.f;
float playerFirerate = 0.25f;
float playerRadius = 14.f;
uint playerMask = 0x00000005;
uint playerSpr;

float enemySpeed = 200.f;
float enemyFirerate = 0.5f;
float enemyRadius = 14.f;
uint enemyMask = 0x00000002;
uint enemySpr;

float bulletSpeed = 1000.f;
float bulletRadius = 6.f;
uint bulletSpr;

uint playerBulletMask = 0x00000002;
uint enemyBulletMask = 0x00000001;

float powerUpSpeed = 350.f;
float powerUpRadius = 6.f;
uint powerUpMask = 0x00000004;

uint powerShieldSpr;
uint shieldSpr;

void load(Application &app) {
  //  app.AddFont((Global::ResPath + "/fonts/invaders.json").c_str());

  //  bulletSpr = app.CreateSprite(
  //      (Global::ResPath + "/images/bullet.png").c_str(), 16, 16, true);

  //  playerSpr = app.CreateSprite(
  //      (Global::ResPath + "/images/cannon.png").c_str(), 32, 32, true);

  //  enemySpr = app.CreateSprite(
  //      (Global::ResPath + "/images/invader.png").c_str(), 32, 32, true);

  //  powerShieldSpr = app.CreateSprite(
  //      (Global::ResPath + "/images/powup_shield.png").c_str(), 24, 24, true);
  //  shieldSpr = app.CreateSprite(
  //      (Global::ResPath + "/images/shield2.png").c_str(), 32, 32, true);
}

void unload(Application &app) {
  //  app.RemoveFont((Global::ResPath + "/fonts/invaders.fnt").c_str());
  //  app.DestroySprite(bulletSpr);
  //  app.DestroySprite(playerSpr);
  //  app.DestroySprite(enemySpr);
}
}

namespace Factory {

Entity newCamera(EntityManager &es, const vec3 &pos, const quat &rot,
                 Application &app) {
  auto e = es.create();
  //  e.assign<TransformCmp>(pos, vec3(1, 1, 1), rot);
  //  auto &camera = app.CreateCamera();
  //  e.assign<CameraCmp>(camera.first, camera.second);
  return e;
}

Entity newParticle(EntityManager &es, Application &app, const vec3 &pos,
                   ParticleCmp::Mode mode, float maxLife,
                   ParticleCmp::ResetFn resetFn, ParticleCmp::UpdateFn updateFn,
                   size_t size) {
  auto e = es.create();
  //  e.assign<TransformCmp>(pos, vec3(1, 1, 1), quat());
  //  e.assign<ParticleCmp>(app, mode, maxLife, resetFn, updateFn, size);
  return e;
}

Entity newPowerUp(EntityManager &es, const vec3 &pos, const vec3 &vel,
                  uint sprite, uint color, float collider, uint mask,
                  PowerCmp::PowerUp power) {
  auto e = es.create();
  //  e.assign<TransformCmp>(pos, vec3(1, 1, 1), quat());
  //  e.assign<PhysicsCmp>(vel, vec3(0, 0, 0), 1, vec3(0, 0, 0));
  //  e.assign<SpriteCmp>(sprite, color);
  //  e.assign<ColliderCmp>(collider, mask);
  //  e.assign<PowerCmp>(power);
  return e;
}

Entity newBullet(EntityManager &es, const vec3 &pos, const quat &rot,
                 const vec3 &vel, uint sprite, uint color, float collider,
                 uint mask, float damage) {
  auto e = es.create();
  //  e.assign<TransformCmp>(pos, vec3(1, 1, 1), rot);
  //  e.assign<PhysicsCmp>(vel, vec3(0, 0, 0), 1, vec3(0, 0, 0));
  //  e.assign<SpriteCmp>(sprite, color);
  //  e.assign<ColliderCmp>(collider, mask);
  //  e.assign<BulletCmp>(damage);
  return e;
}

Entity newPlayer(EntityManager &es, const vec3 &pos, uchar lives) {
  auto e = es.create();
  //  e.assign<TransformCmp>(pos, vec3(1, 1, 1), quat());
  //  e.assign<PhysicsCmp>(vec3(0, 0, 0), vec3(0, 0, 0), 1, vec3(0, 0, 0));
  //  e.assign<SpriteCmp>(GVars::playerSpr, 0xFFFFFFFF);
  //  e.assign<ColliderCmp>(GVars::playerRadius, GVars::playerMask);
  //  e.assign<HealthCmp>(1, 0, 1);
  //  e.assign<PlayerCmp>(GVars::playerFirerate, GVars::playerSpeed, lives);
  return e;
}

Entity newEnemy(EntityManager &es, float speed, uint points, uint color,
                const vec3 &idxPos) {
  auto e = es.create();
  //  e.assign<TransformCmp>(vec3(0, 0, 0), vec3(1, 1, 1), quat());
  //  e.assign<PhysicsCmp>(vec3(0, 0, 0), vec3(0, 0, 0), 1, vec3(0, 0, 0));
  //  e.assign<SpriteCmp>(GVars::enemySpr, color);
  //  e.assign<ColliderCmp>(GVars::enemyRadius, GVars::enemyMask);
  //  e.assign<HealthCmp>(1, 0, 1);
  //  e.assign<EnemyCmp>(GVars::enemyFirerate, speed, points, idxPos);
  return e;
}
}

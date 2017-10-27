#pragma once

#include "Components.hpp"
#include <entityx/entityx.h>
#include <glm/glm.hpp>

using namespace entityx;
using namespace glm;

namespace GVars {

extern float playerSpeed;
extern float playerFirerate;
extern float playerRadius;
extern uint playerMask;
extern uint playerSpr;

extern float enemySpeed;
extern float enemyFirerate;
extern float enemyRadius;
extern uint enemyMask;
extern uint enemySpr;

extern float bulletSpeed;
extern float bulletRadius;
extern uint bulletSpr;

extern uint playerBulletMask;
extern uint enemyBulletMask;

extern float powerUpSpeed;
extern float powerUpRadius;
extern uint powerUpMask;

extern uint powerShieldSpr;
extern uint shieldSpr;

extern void load(Application &);
extern void unload(Application &);
}

namespace Factory {

/**
 * @brief newCamera
 * @param es
 * @param pos
 * @param rot
 * @param app
 * @return
 */
Entity newCamera(EntityManager &es, const vec3 &pos, const quat &rot,
                 Application &app);

/**
 * @brief newParticle
 * @param es
 * @param pos
 * @param maxLife
 * @param mode
 * @param resetFn
 * @param updateFn
 * @param size
 * @return
 */
Entity newParticle(EntityManager &es, Application &app, const vec3 &pos,
                   ParticleCmp::Mode mode, float maxLife,
                   ParticleCmp::ResetFn resetFn, ParticleCmp::UpdateFn updateFn,
                   size_t size);

/**
 * @brief newPowerUp
 * @param es
 * @param pos
 * @param vel
 * @param sprite
 * @param color
 * @param collider
 * @param mask
 * @return
 */
Entity newPowerUp(EntityManager &es, const vec3 &pos, const vec3 &vel,
                  uint sprite, uint color, float collider, uint mask,
                  PowerCmp::PowerUp power);

/**
 * @brief Spawns a new bullet entity
 * @param es, reference
 * @param pos, of entity
 * @param vel, of entity
 * @param sprite, to use
 * @param color, to tint
 * @param collider, body
 * @param damage, on hit
 * @return entity handle
 */
Entity newBullet(EntityManager &es, const vec3 &pos, const quat &rot,
                 const vec3 &vel, uint sprite, uint color, float collider,
                 uint mask, float damage);

/**
 * @brief newPlayer
 * @param es
 * @param pos
 * @param lives
 * @return
 */
Entity newPlayer(EntityManager &es, const vec3 &pos, uchar lives);

/**
 * @brief newEnemy
 * @param es
 * @param speed
 * @param points
 * @param color
 * @param idxPos
 * @return
 */
Entity newEnemy(EntityManager &es, float speed, uint points, uint color,
                const vec3 &idxPos);
}

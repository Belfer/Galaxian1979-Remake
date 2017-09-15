#pragma once

#include "Components.hpp"
#include <entityx/entityx.h>
#include <glm/glm.hpp>

using namespace entityx;
using namespace glm;

namespace Factory {

/**
 * @brief newParticle
 * @param entityMgr
 * @param pos
 * @param maxLife
 * @param mode
 * @param resetFn
 * @param updateFn
 * @param size
 * @return
 */
Entity newParticle(EntityManager &entityMgr, const vec2 &pos,
                   ParticleCmp::Mode mode, float maxLife,
                   ParticleCmp::ResetFn resetFn, ParticleCmp::UpdateFn updateFn,
                   size_t size);

/**
 * @brief Spawns a new bullet entity
 * @param entityMgr, reference
 * @param pos, of entity
 * @param vel, of entity
 * @param sprite, to use
 * @param collider, body
 * @param damage, on hit
 * @return entity handle
 */
Entity newBullet(EntityManager &entityMgr, const vec2 &pos, const vec2 &vel,
                 unsigned int sprite, unsigned int color, float collider,
                 float damage);

/**
 * @brief Spawns a new player entity
 * @param entityMgr
 * @param pos
 * @param sprite
 * @param collider
 * @param bulletSpr
 * @param fireRate
 * @param speed
 * @param lives
 * @return
 */
Entity newPlayer(EntityManager &entityMgr, const vec2 &pos, unsigned int sprite,
                 float collider, unsigned int bulletSpr, float fireRate,
                 float speed, unsigned char lives);

/**
 * @brief newEnemy
 * @param entityMgr
 * @param sprite
 * @param collider
 * @param bulletSpr
 * @param fireRate
 * @param speed
 * @param points
 * @param color
 * @param idxPos
 * @return
 */
Entity newEnemy(EntityManager &entityMgr, unsigned int sprite, float collider,
                unsigned int bulletSpr, float fireRate, float speed,
                unsigned int points, unsigned int color, const vec2 &idxPos);
}

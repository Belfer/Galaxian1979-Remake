#pragma once

#include <Application.h>
#include <entityx/entityx.h>
#include <glm/glm.hpp>

using namespace NHTV;
using namespace entityx;
using namespace glm;

/**
 * @brief TransformCmp, canonical position, scale and rotation
 */
struct TransformCmp {
  TransformCmp() {}
  TransformCmp(const vec2 &pos, const vec2 &scl, float rot)
      : pos(pos), scl(scl), rot(rot) {}
  vec2 pos = vec2(0, 0);
  vec2 scl = vec2(1, 1);
  float rot = 0;
};

/**
 * @brief PhysicsCmp, represents physical vectors for movement
 */
struct PhysicsCmp {
  PhysicsCmp() {}
  PhysicsCmp(const vec2 &vel, const vec2 &acc, float mass, const vec2 &force)
      : vel(vel), acc(acc), mass(mass), force(force) {}

  vec2 vel = vec2(0, 0);
  vec2 acc = vec2(0, 0);

  float mass = 0;
  vec2 force = vec2(0, 0);
};

/**
 * @brief ParticleCmp
 */
struct ParticleCmp {
  enum Mode { LOOP, ONCE };

  struct Particle;
  using ResetFn = std::function<void(Particle &)>;
  using UpdateFn = std::function<void(float, float, std::vector<Particle> &)>;

  ParticleCmp(Mode mode, float maxLife, ResetFn resetFn, UpdateFn updateFn,
              size_t size)
      : mode(mode), maxLife(maxLife), resetFn(resetFn), updateFn(updateFn) {
    particles.resize(size);
    for (auto &p : particles) {
      resetFn(p);
    }
  }

  struct Particle {
    Particle() {}
    std::pair<vec2, vec2> pos;
    vec2 vel = vec2(0, 0);
    float time = 0;
    float life = 1;
    float zdepth = 1;
    SColour color = 0xFFFFFFFF;
  };

  Mode mode = LOOP;
  float maxLife = 1;
  ResetFn resetFn;
  UpdateFn updateFn;
  std::vector<Particle> particles;
};

/**
 * @brief SpriteCmp, keeps handle of sprite ID
 */
struct SpriteCmp {
  SpriteCmp(unsigned int handle, unsigned int color)
      : handle(handle), color(color) {}

  const unsigned int handle;
  unsigned int color;
};

/**
 * @brief ColliderCmp, float that represents circle radius
 */
struct ColliderCmp {
  ColliderCmp() {}
  ColliderCmp(float radius) : radius(radius) {}
  float radius = 1;
};

/**
 * @brief HealthCmp, system caps and checks a number between a min and max
 */
struct HealthCmp {
  HealthCmp() {}
  HealthCmp(float val, float min, float max) : val(val), min(min), max(max) {}

  float val = 100;
  float min = 0;
  float max = 100;
};

/**
 * @brief TextCmp, holds a string, font size and color to be rendered
 */
struct TextCmp {
  TextCmp() {}
  TextCmp(const std::string &textStr, float fontSize, unsigned int color)
      : textStr(textStr), fontSize(fontSize), color(color) {}

  std::string textStr;
  float fontSize;
  unsigned int color = 0xFFFFFFFF;
};

/**
 * @brief LineCmp, holds two points and color to be rendered
 */
struct LineCmp {
  LineCmp() {}
  LineCmp(const vec2 &pA, const vec2 &pB, unsigned int color)
      : pA(pA), pB(pB), color(color) {}

  vec2 pA;
  vec2 pB;
  unsigned int color = 0xFFFFFFFF;
};

/**
 * @brief BulletCmp, values of the effect of a bullet
 */
struct BulletCmp {
  BulletCmp(float damage) : damage(damage) {}
  float damage;
};

/**
 * @brief PlayerCmp, player variables
 */
struct PlayerCmp {
  PlayerCmp() {}
  PlayerCmp(unsigned int bulletSpr, float fireRate, float speed,
            unsigned char lives)
      : bulletSpr(bulletSpr), fireRate(fireRate), speed(speed), lives(lives),
        fireTimer(0) {}

  unsigned int bulletSpr = -1;
  float fireRate = 1;
  float speed = 500;
  unsigned char lives = 3;

private:
  friend class PlayerSystem;
  float fireTimer = 0;
  vec2 dir;
};

/**
 * @brief EnemyCmp, enemy variables
 */
struct EnemyCmp {
  EnemyCmp() {}
  EnemyCmp(unsigned int bulletSpr, float fireRate, float speed,
           unsigned int points, const vec2 &idxPos)
      : bulletSpr(bulletSpr), fireRate(fireRate), speed(speed), points(points),
        state(IDLE), idxPos(idxPos), fireTimer(0) {}

  unsigned int bulletSpr = -1;
  float fireRate = 1;
  float speed = 500;
  unsigned int points = 1;

  enum AIState { IDLE, UNGROUPING, ATTACKING, FLEEING, GROUPING };
  AIState state = IDLE;

private:
  friend class EnemySystem;
  vec2 idxPos;
  float fireTimer = 0;
};

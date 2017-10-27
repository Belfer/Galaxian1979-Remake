#pragma once

#include "Core/Color.hpp"
#include "Core/Engine.hpp"
#include <entityx/entityx.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace NHTV;
using namespace entityx;
using namespace glm;

struct EntityCmp {
  EntityCmp(const std::string &name, const std::string &tag)
      : name(name), tag(tag) {}
  std::string name;
  std::string tag;
};

/**
 * @brief The CameraCmp struct
 */
struct CameraCmp {
  CameraCmp(size_t id /*, Camera &camera*/) : id(id) {} //, camera(camera) {}

  const size_t id;
  // Camera &camera;
};

/**
 * @brief TransformCmp, canonical position, scale and rotation
 */
struct TransformCmp {
  TransformCmp() {}
  TransformCmp(const vec3 &pos, const vec3 &scl, const quat &rot)
      : pos(pos), scl(scl), rot(rot) {}

  vec3 pos = vec3(0);
  vec3 scl = vec3(1);
  quat rot = quat();
};

/**
 * @brief PhysicsCmp, represents physical vectors for movement
 */
struct PhysicsCmp {
  PhysicsCmp() {}
  PhysicsCmp(const vec3 &vel, float trq,
             float mass) //, const vec3 &acc, const vec3 &force)
      : vel(vel),
        trq(trq),
        mass(mass) {} //, acc(acc), force(force) {}

  vec3 vel = vec3(0);
  float trq = 0;
  // vec3 acc = vec3(0);

  float mass = 0;
  // vec3 force = vec3(0);
};

/**
 * @brief ParticleCmp
 */
struct ParticleCmp {
  enum Mode { LOOP, ONCE };

  struct Particle;
  using ResetFn = std::function<void(Application &, Particle &)>;
  using UpdateFn = std::function<void(float, float, std::vector<Particle> &)>;

  ParticleCmp(Application &app, Mode mode, float maxLife, ResetFn resetFn,
              UpdateFn updateFn, size_t size)
      : mode(mode), maxLife(maxLife), resetFn(resetFn), updateFn(updateFn) {
    particles.resize(size);
    for (auto &p : particles) {
      resetFn(app, p);
    }
  }

  struct Particle {
    Particle() {}
    std::pair<vec3, vec3> pos;
    vec3 vel = vec3(0);
    float time = 0;
    float life = 1;
    float zdepth = 1;
    Color color = 0xFFFFFFFF;
  };

  Mode mode = LOOP;
  float maxLife = 1;
  ResetFn resetFn;
  UpdateFn updateFn;
  std::vector<Particle> particles;
};

/**
 * @brief PowerUpCmp, power ups
 */
struct PowerCmp {
  enum PowerUp {
    NONE = 0,
    SHIELD = 1,
    DOUBLE_SHOT = 2,
    RAPID_FIRE = 4,
    LAZER_BLAST = 8
  };

  PowerCmp() {}
  PowerCmp(PowerUp power) : power(power) {}

  PowerUp power = NONE;
};

struct MaterialCmp {
  MaterialCmp() {}
  MaterialCmp(size_t texture, size_t shader)
      : texture(texture), shader(shader) {}
  size_t texture;
  size_t shader;
};

/**
 * @brief SpriteCmp, keeps handle of sprite ID
 */
struct SpriteCmp {
  SpriteCmp() {}
  SpriteCmp(const vec2 &origin, const vec2 &size, const vec4 &uv)
      : origin(origin), size(size), uvRect(uvRect) {}

  vec2 origin = vec2(0, 0);
  vec2 size = vec2(1, 1);
  vec4 uvRect = vec4(0, 0, 1, 1);
};

/**
 * @brief ColliderCmp, float that represents circle radius
 */
struct ColliderCmp {
  ColliderCmp() {}
  ColliderCmp(float radius, uint mask) : radius(radius), mask(mask) {}
  float radius = 1;
  uint mask = 0;
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
  TextCmp(const std::string &textStr, float fontSize, uint color)
      : textStr(textStr), fontSize(fontSize), color(color) {}

  std::string textStr;
  float fontSize;
  uint color = 0xFFFFFFFF;
};

/**
 * @brief LineCmp, holds two points and color to be rendered
 */
struct LineCmp {
  LineCmp() {}
  LineCmp(const vec3 &pA, const vec3 &pB, uint color)
      : pA(pA), pB(pB), color(color) {}

  vec3 pA;
  vec3 pB;
  uint color = 0xFFFFFFFF;
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
  PlayerCmp(float fireRate, float speed, uchar lives)
      : fireRate(fireRate), speed(speed), lives(lives), fireTimer(0) {}

  float fireRate = 1;
  float speed = 500;
  uchar lives = 3;

private:
  friend class PlayerSystem;
  vec3 erot = vec3(0);

  float fireTimer = 0;
  vec3 forward;
};

/**
 * @brief EnemyCmp, enemy variables
 */
struct EnemyCmp {
  EnemyCmp() {}
  EnemyCmp(float fireRate, float speed, uint points, const vec3 &idxPos)
      : fireRate(fireRate), speed(speed), points(points), state(IDLE),
        idxPos(idxPos), fireTimer(0) {}

  float fireRate = 1;
  float speed = 500;
  uint points = 1;

  enum AIState { IDLE, UNGROUPING, ATTACKING, FLEEING, GROUPING };
  AIState state = IDLE;

private:
  friend class EnemySystem;
  vec3 idxPos;
  float fireTimer = 0;
};

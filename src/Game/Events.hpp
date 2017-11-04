#pragma once

#include "Components.hpp"
#include "Engine/Core/Engine.hpp"
#include <entityx/entityx.h>
#include <glm/glm.hpp>

using namespace NHTV;
using namespace entityx;
using namespace glm;

using Collision = std::pair<Entity, Entity>;
struct CollisionEvent {
  CollisionEvent(const Collision &collision) : collision(collision) {}
  mutable Collision collision;
};

struct EntityRemovedEvent {
  EntityRemovedEvent(const Entity &entity) : entity(entity) {}
  const Entity entity;
};

struct GameResetEvent {};

struct ExplosionEvent {
  ExplosionEvent(const vec3 &pos) : pos(pos) {}
  const vec3 pos;
};

struct PlayerKilledEvent {};

struct PowerEnableEvent {
  PowerEnableEvent(PowerCmp::PowerUp power) : power(power) {}
  const PowerCmp::PowerUp power;
};

struct PowerDisableEvent {
  PowerDisableEvent(PowerCmp::PowerUp power) : power(power) {}
  const PowerCmp::PowerUp power;
};

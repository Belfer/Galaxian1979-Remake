#pragma once

#include <Application.h>
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
  ExplosionEvent(const vec2 &pos) : pos(pos) {}
  const vec2 pos;
};

struct PlayerKilledEvent {};

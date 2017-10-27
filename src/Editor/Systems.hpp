#pragma once

#include "Core/Engine.hpp"
#include <entityx/entityx.h>
#include <glm/glm.hpp>

using namespace NHTV;
using namespace entityx;
using namespace glm;

/**
 * @brief Base class for app systems
 */
template <typename T> struct GalaxianSystem : public System<T> {
  GalaxianSystem(EntityManager &es, EventManager &ev, SystemManager &sm)
      : m_entities(es), m_events(ev), m_systems(sm) {}

  virtual void update(EntityManager &es, EventManager &events,
                      TimeDelta dt) = 0;

protected:
  EntityManager &m_entities;
  EventManager &m_events;
  SystemManager &m_systems;
};

#define DEFINE_SYSTEM(className)                                               \
  className(EntityManager &es, EventManager &ev, SystemManager &sm)            \
      : GalaxianSystem(es, ev, sm) {}

/**
 * @brief The PhysicsSystem class
 */
class PhysicsSystem : public GalaxianSystem<PhysicsSystem> {
public:
  DEFINE_SYSTEM(PhysicsSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

/**
 * @brief The SpriteSystem class
 */
class SpriteSystem : public GalaxianSystem<SpriteSystem> {
public:
  DEFINE_SYSTEM(SpriteSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

/*#include "Game/Systems.hpp"

class DebugSystem : public GalaxianSystem<DebugSystem> {
public:
  DEFINE_SYSTEM(DebugSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};*/

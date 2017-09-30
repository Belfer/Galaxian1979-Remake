#pragma once

#include "Events.hpp"
#include "Framework/Application.h"
#include <entityx/entityx.h>
#include <glm/glm.hpp>

using namespace NHTV;
using namespace entityx;
using namespace glm;

/**
 * @brief Base class for app systems
 */
template <typename T> struct GalaxianSystem : public System<T> {
  GalaxianSystem(Application &app, EntityManager &es, EventManager &ev,
                 SystemManager &sm)
      : m_app(app), m_entities(es), m_events(ev), m_systems(sm) {}

  virtual void update(EntityManager &es, EventManager &events,
                      TimeDelta dt) = 0;

protected:
  Application &m_app;
  EntityManager &m_entities;
  EventManager &m_events;
  SystemManager &m_systems;
};

#define DEFINE_SYSTEM(className)                                               \
  className(Application &app, EntityManager &es, EventManager &ev,             \
            SystemManager &sm)                                                 \
      : GalaxianSystem(app, es, ev, sm) {}

/**
 * @brief The HealthSystem class
 */
class HealthSystem : public GalaxianSystem<HealthSystem> {
public:
  DEFINE_SYSTEM(HealthSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

/**
 * @brief The CameraSystem class
 */
class CameraSystem : public GalaxianSystem<CameraSystem> {
public:
  DEFINE_SYSTEM(CameraSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

/**
 * @brief The PhysicsSystem class
 */
class PhysicsSystem : public GalaxianSystem<PhysicsSystem> {
public:
  DEFINE_SYSTEM(PhysicsSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

/**
 * @brief The CollisionSystem class
 */
class CollisionSystem : public GalaxianSystem<CollisionSystem>,
                        public Receiver<CollisionSystem> {
public:
  DEFINE_SYSTEM(CollisionSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
  // void receive(const CollisionEvent &e);

  void configure(EventManager &ev) {
    // es.subscribe<CollisionEvent>(*this);
  }
};

/**
 * @brief The EntitySpawnerSystem class
 */
class SpawnSystem : public GalaxianSystem<SpawnSystem>,
                    public Receiver<SpawnSystem> {
public:
  DEFINE_SYSTEM(SpawnSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override {}
  void receive(const GameResetEvent &e);
  void receive(const ExplosionEvent &e);
  void receive(const EntityRemovedEvent &e);

  void configure(EventManager &ev) {
    ev.subscribe<GameResetEvent>(*this);
    ev.subscribe<ExplosionEvent>(*this);
    ev.subscribe<EntityRemovedEvent>(*this);
  }
};

/**
 * @brief The PlayerSystem class
 */
class PlayerSystem : public GalaxianSystem<PlayerSystem>,
                     public Receiver<PlayerSystem> {
public:
  DEFINE_SYSTEM(PlayerSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
  void receive(const EntityRemovedEvent &e);
  void receive(const CollisionEvent &e);
  void receive(const PowerEnableEvent &e);
  void receive(const PowerDisableEvent &e);

  void configure(EventManager &ev) {
    ev.subscribe<EntityRemovedEvent>(*this);
    ev.subscribe<CollisionEvent>(*this);
    ev.subscribe<PowerEnableEvent>(*this);
    ev.subscribe<PowerDisableEvent>(*this);
  }

private:
  Entity m_player;
  PowerCmp::PowerUp m_power = PowerCmp::NONE;
};

/**
 * @brief The EnemySystem class
 */
class EnemySystem : public GalaxianSystem<EnemySystem>,
                    public Receiver<EnemySystem> {
public:
  DEFINE_SYSTEM(EnemySystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
  void receive(const GameResetEvent &e);

  void configure(EventManager &ev) { ev.subscribe<GameResetEvent>(*this); }

private:
  vec3 m_groupPos;
  float m_groupSpeed = 200;
  float m_groupDir = 1;
  float m_attackTimer = 0;
  float m_enemyTimer = 0;
  uint m_enemiesAttaking = 0;
  uint m_enemyCount = 0;
  bool m_attacking = false;
};

/**
 * @brief The PowerUpSystem class
 */
class PowerUpSystem : public GalaxianSystem<PowerUpSystem>,
                      public Receiver<PowerUpSystem> {
public:
  DEFINE_SYSTEM(PowerUpSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
  void receive(const GameResetEvent &e);
  void receive(const CollisionEvent &e);
  void receive(const PowerEnableEvent &e);
  void receive(const PowerDisableEvent &e);

  void configure(EventManager &ev) {
    ev.subscribe<GameResetEvent>(*this);
    ev.subscribe<CollisionEvent>(*this);
    ev.subscribe<PowerEnableEvent>(*this);
    ev.subscribe<PowerDisableEvent>(*this);
  }

private:
  PowerCmp::PowerUp m_power = PowerCmp::NONE;
  bool m_powerActive = false;
  float m_shieldTimer = 0.f;
  float m_doubleShotTimer = 0.f;
};

/**
 * @brief The BulletSystem class
 */
class BulletSystem : public GalaxianSystem<BulletSystem>,
                     public Receiver<BulletSystem> {
public:
  DEFINE_SYSTEM(BulletSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
  void receive(const CollisionEvent &e);

  void configure(EventManager &ev) { ev.subscribe<CollisionEvent>(*this); }
};

/**
 * @brief The MenuSystem class
 */
class MenuSystem : public GalaxianSystem<MenuSystem>,
                   public Receiver<MenuSystem> {
public:
  DEFINE_SYSTEM(MenuSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
  void receive(const GameResetEvent &e);
  void receive(const EntityRemovedEvent &e);

  void configure(EventManager &ev) {
    ev.subscribe<GameResetEvent>(*this);
    ev.subscribe<EntityRemovedEvent>(*this);
  }

private:
  uint m_score = 0;
  uint m_highscore = 0;
};

/**
 * @brief The ParticleSystem class
 */
class ParticleSystem : public GalaxianSystem<ParticleSystem> {
public:
  DEFINE_SYSTEM(ParticleSystem)
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

/**
 * @brief The TextSystem class
 */
class TextSystem : public GalaxianSystem<TextSystem> {
public:
  DEFINE_SYSTEM(TextSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

/**
 * @brief The LineSystem class
 */
class LineSystem : public GalaxianSystem<LineSystem> {
public:
  DEFINE_SYSTEM(LineSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

#pragma once

#include "Events.hpp"
#include <Application.h>
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

  void configure(EventManager &es) {
    // es.subscribe<CollisionEvent>(*this);
  }
};

/**
 * @brief The EntitySpawnerSystem class
 */
class EntitySpawnerSystem : public GalaxianSystem<EntitySpawnerSystem>,
                            public Receiver<EntitySpawnerSystem> {
public:
  struct EntitySpawnerVars;

  EntitySpawnerSystem(Application &app, EntityManager &es, EventManager &ev,
                      SystemManager &sm, const EntitySpawnerVars &vars)
      : GalaxianSystem(app, es, ev, sm), m_vars(vars) {}
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
  void receive(const GameResetEvent &e);
  void receive(const ExplosionEvent &e);

  void configure(EventManager &es) {
    es.subscribe<GameResetEvent>(*this);
    es.subscribe<ExplosionEvent>(*this);
  }

  struct EntitySpawnerVars {
    float playerSpeed;
    float playerFirerate;

    float enemySpeed;
    float enemyFirerate;

    unsigned int bulletSpr;
    unsigned int playerSpr;
    unsigned int enemySpr;
  };

private:
  EntitySpawnerVars m_vars;
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

  void configure(EventManager &es) { es.subscribe<EntityRemovedEvent>(*this); }
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

  void configure(EventManager &es) { es.subscribe<GameResetEvent>(*this); }

private:
  vec2 m_groupPos;
  float m_groupSpeed = 200;
  float m_groupDir = 1;
  float m_attackTimer = 0;
  float m_enemyTimer = 0;
  unsigned int m_enemiesAttaking = 0;
  unsigned int m_enemyCount = 0;
  bool m_attacking = false;
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

  void configure(EventManager &es) { es.subscribe<CollisionEvent>(*this); }
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

  void configure(EventManager &es) {
    es.subscribe<GameResetEvent>(*this);
    es.subscribe<EntityRemovedEvent>(*this);
  }

private:
  unsigned int m_score = 0;
  unsigned int m_highscore = 0;
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

/**
 * @brief The DebugSystem class
 */
class DebugSystem : public GalaxianSystem<DebugSystem> {
public:
  DEFINE_SYSTEM(DebugSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;

private:
  float m_fpsTimer = 0;
  unsigned int m_fps = 0;
  std::string m_fpsStr;
};

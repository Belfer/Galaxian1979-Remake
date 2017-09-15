#include "GalaxianApp.hpp"
#include "Factory.hpp"
#include "Systems.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

APPLICATION_ENTRY(GalaxianApp)
#define ENABLE_DEBUG 1

#if (ENABLE_DEBUG)
bool keytmp = true;
bool debbuging = false;
bool slowmo = false;
bool postproc = true;
#endif

EntitySpawnerSystem::EntitySpawnerVars spawnerVars;

void GalaxianApp::receive(const PlayerKilledEvent &e) {
  m_timeDilation = 0.25f;
}

void GalaxianApp::receive(const GameResetEvent &e) { m_timeDilation = 1.f; }

bool GalaxianApp::onCreate(int argc, char **argv) {
  srand(time(NULL));

  setBackgroundColor(SColour(0x0C, 0x0C, 0x0C, 0xFF));

  spawnerVars.playerSpeed = 300.f;
  spawnerVars.playerFirerate = 0.25f;

  spawnerVars.enemySpeed = 200.f;
  spawnerVars.enemyFirerate = 0.5f;

  // Load resources
  SetPostProcessFx("../resources/shaders/bloom.fsh");
  SetColorPostFxVar(vec4(1, 1, 1, 1));

  AddFont("../resources/fonts/invaders.fnt");

  spawnerVars.bulletSpr =
      CreateSprite("../resources/images/bullet.png", 16, 16, true);
  spawnerVars.playerSpr =
      CreateSprite("../resources/images/cannon.png", 32, 32, true);
  spawnerVars.enemySpr =
      CreateSprite("../resources/images/invader.png", 32, 32, true);

  // Configure event receivers
  configure(events);

  // Add systems
  systems.add<HealthSystem>(*this, entities, events, systems);
  systems.add<PhysicsSystem>(*this, entities, events, systems);
  systems.add<CollisionSystem>(*this, entities, events, systems);
  systems.add<PlayerSystem>(*this, entities, events, systems);
  systems.add<EnemySystem>(*this, entities, events, systems);
  systems.add<BulletSystem>(*this, entities, events, systems);

  systems.add<EntitySpawnerSystem>(*this, entities, events, systems,
                                   spawnerVars);

  systems.add<MenuSystem>(*this, entities, events, systems);
  systems.add<ParticleSystem>(*this, entities, events, systems);
  systems.add<SpriteSystem>(*this, entities, events, systems);
  systems.add<TextSystem>(*this, entities, events, systems);
  systems.add<LineSystem>(*this, entities, events, systems);
  systems.add<DebugSystem>(*this, entities, events, systems);
  systems.configure();

  // Reset level
  events.emit<GameResetEvent>();
  return true;
}

void GalaxianApp::onUpdate(float deltaTime) {
  // Quit app when escape is pressed
  if (IsKeyDown(KEY_ESCAPE))
    destroy();

#if (ENABLE_DEBUG)
  if (IsKeyDown(KEY_D)) {
    if (keytmp) {
      debbuging = !debbuging;
      keytmp = false;
    }
  } else if (IsKeyDown(KEY_R)) {
    if (keytmp) {
      events.emit<GameResetEvent>();
      keytmp = false;
    }
  } else if (IsKeyDown(KEY_S)) {
    if (keytmp) {
      slowmo = !slowmo;
      if (slowmo)
        m_timeDilation = 0.25f;
      else
        m_timeDilation = 1.f;
      keytmp = false;
    }
  } else if (IsKeyDown(KEY_P)) {
    if (keytmp) {
      postproc = !postproc;
      SetPostProcessEnabled(postproc);
      keytmp = false;
    }
  } else {
    keytmp = true;
  }
#endif

  const float dt = deltaTime * m_timeDilation;
  systems.update<HealthSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<CollisionSystem>(dt);
  systems.update<PlayerSystem>(dt);
  systems.update<EnemySystem>(dt);
  systems.update<BulletSystem>(dt);

  systems.update<EntitySpawnerSystem>(dt);
}

void GalaxianApp::onDraw() {
  clearScreen();

  // Update render systems
  const float dt = GetDeltaTime() * m_timeDilation;
  systems.update<MenuSystem>(dt);
  systems.update<SpriteSystem>(dt);
  systems.update<ParticleSystem>(dt);
  systems.update<TextSystem>(dt);
  systems.update<LineSystem>(dt);

#if (ENABLE_DEBUG)
  if (debbuging)
    systems.update<DebugSystem>(dt);
#endif
}

void GalaxianApp::onDestroy() {
  // Unload resources
  RemoveFont("../resources/fonts/invaders.fnt");
  DestroySprite(spawnerVars.bulletSpr);
  DestroySprite(spawnerVars.playerSpr);
  DestroySprite(spawnerVars.enemySpr);
}

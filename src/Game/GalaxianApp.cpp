#include "GalaxianApp.hpp"
#include "Factory.hpp"
#include "Framework/Config.h"
#include "Systems.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <imgui.h>

void GalaxianApp::receive(const PlayerKilledEvent &e) { m_slowmo = true; }

void GalaxianApp::receive(const GameResetEvent &e) {
  ClearCameras();
  m_slowmo = false;
}

void GalaxianApp::receive(const ExplosionEvent &e) {
  m_time = 0;
  SetPositionPostFxVar(vec4(e.pos.x / 512.f, 1 - (e.pos.y / 1024.f), 0, 0));
}

bool GalaxianApp::onCreate(int argc, char **argv) {
  srand(time(NULL));

  // Load resources
  SetPostProcessFx((Config::g_ResourcesPath + "/shaders/postpass.fsh").c_str());
  SetColorPostFxVar(vec4(1, 1, 1, 1));

  setBackgroundColor(SColour(0x0C, 0x0C, 0x0C, 0xFF));

  GVars::load(*this);

  // Configure event receiversr
  configure(events);

  // Add systems
  addSystems();
  systems.configure();

  // Reset level
  events.emit<GameResetEvent>();
  return true;
}

void GalaxianApp::onUpdate(float deltaTime) {
  // Quit app when escape is pressed
  if (IsKeyDown(KEY_ESCAPE))
    destroy();

  if (m_paused)
    m_timeDilation = Lerp(m_timeDilation, 0.f, 0.1f);
  else if (m_slowmo)
    m_timeDilation = Lerp(m_timeDilation, SLOWMO_TD, 0.05f);
  else
    m_timeDilation = Lerp(m_timeDilation, 1.f, 0.1f);

  SetPostProcessEnabled(m_postproc);
  SetTimePostFxVar(m_time);

  // Update logic systems
  const float dt = deltaTime * m_timeDilation;
  m_time += dt;
  updateSystems(dt);
}

void GalaxianApp::onDraw() {
  clearScreen();

  // Update render systems
  const float dt = GetDeltaTime() * m_timeDilation;
  renderSystems(dt);
}

void GalaxianApp::onEditor() {}

void GalaxianApp::onDestroy() {
  // Unload resources
  GVars::unload(*this);
}

void GalaxianApp::addSystems() {
  // Add systems
  systems.add<HealthSystem>(*this, entities, events, systems);
  systems.add<PhysicsSystem>(*this, entities, events, systems);
  systems.add<CollisionSystem>(*this, entities, events, systems);
  systems.add<PlayerSystem>(*this, entities, events, systems);
  systems.add<EnemySystem>(*this, entities, events, systems);
  systems.add<BulletSystem>(*this, entities, events, systems);
  systems.add<PowerUpSystem>(*this, entities, events, systems);
  systems.add<SpawnSystem>(*this, entities, events, systems);

  systems.add<MenuSystem>(*this, entities, events, systems);
  systems.add<ParticleSystem>(*this, entities, events, systems);
  systems.add<SpriteSystem>(*this, entities, events, systems);
  systems.add<TextSystem>(*this, entities, events, systems);
  systems.add<LineSystem>(*this, entities, events, systems);
}

void GalaxianApp::updateSystems(float dt) {
  // Update logic systems
  systems.update<HealthSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<CollisionSystem>(dt);
  systems.update<PlayerSystem>(dt);
  systems.update<EnemySystem>(dt);
  systems.update<BulletSystem>(dt);
  systems.update<PowerUpSystem>(dt);
  systems.update<SpawnSystem>(dt);
}

void GalaxianApp::renderSystems(float dt) {
  // Update render systems
  systems.update<MenuSystem>(dt);
  systems.update<SpriteSystem>(dt);
  systems.update<ParticleSystem>(dt);
  systems.update<TextSystem>(dt);
  systems.update<LineSystem>(dt);
}

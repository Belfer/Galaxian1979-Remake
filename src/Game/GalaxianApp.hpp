#pragma once

#include "Events.hpp"
#include "Engine/Core/Engine.hpp"
#include <entityx/entityx.h>

using namespace NHTV;
using namespace entityx;

#define ENABLE_DEBUG 1
#define SLOWMO_TD 0.25f

/**
 * @brief Derived application class that wraps up all globals neatly
 */
class GalaxianApp : public Application,
                    public EntityX,
                    public Receiver<GalaxianApp> {
protected:
  virtual bool onCreate(int argc, char **argv);
  virtual void onUpdate(float dt);
  virtual void onDraw(float dt);
  virtual void onEditor();
  virtual void onDestroy();

  virtual void load();
  virtual void addSystems();
  virtual void updateSystems(float dt);
  virtual void renderSystems(float dt);

  float m_timeScale = 1.f;
  float m_time = 0;
  bool m_paused = false;
  bool m_slowmo = false;
  bool m_postproc = true;

public:
  GalaxianApp() {
    // Configure event receiversr
    configure(events);
  }

  void configure(EventManager &es) {
    es.subscribe<PlayerKilledEvent>(*this);
    es.subscribe<GameResetEvent>(*this);
    es.subscribe<ExplosionEvent>(*this);
  }

  void receive(const PlayerKilledEvent &e);
  void receive(const GameResetEvent &e);
  void receive(const ExplosionEvent &e);
};

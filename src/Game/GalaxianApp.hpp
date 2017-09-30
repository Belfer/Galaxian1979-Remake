#pragma once

#include "Events.hpp"
#include "Framework/Application.h"
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
  virtual void onDraw();
  virtual void onEditor();
  virtual void onDestroy();

  void addSystems();
  void updateSystems(float dt);
  void renderSystems(float dt);

  float m_timeDilation = 1.f;
  float m_time = 0;
  bool m_paused = false;
  bool m_slowmo = false;
  bool m_postproc = true;

public:
  inline void setTimeDialation(float td) { m_timeDilation = td; }

  void configure(EventManager &es) {
    es.subscribe<PlayerKilledEvent>(*this);
    es.subscribe<GameResetEvent>(*this);
    es.subscribe<ExplosionEvent>(*this);
  }

  void receive(const PlayerKilledEvent &e);
  void receive(const GameResetEvent &e);
  void receive(const ExplosionEvent &e);
};

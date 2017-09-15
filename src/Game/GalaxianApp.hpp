#pragma once

#include "Events.hpp"
#include <Application.h>
#include <entityx/entityx.h>

using namespace NHTV;
using namespace entityx;

/**
 * @brief Derived application class that wraps up all globals neatly
 */
class GalaxianApp : public Application,
                    public EntityX,
                    public Receiver<GalaxianApp> {
  virtual bool onCreate(int argc, char **argv);
  virtual void onUpdate(float dt);
  virtual void onDraw();
  virtual void onDestroy();

  float m_timeDilation = 1.f;

public:
  inline void setTimeDialation(float td) { m_timeDilation = td; }

  void configure(EventManager &es) {
    es.subscribe<PlayerKilledEvent>(*this);
    es.subscribe<GameResetEvent>(*this);
  }

  void receive(const PlayerKilledEvent &e);
  void receive(const GameResetEvent &e);
};

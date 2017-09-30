#pragma once

#include "Game/GalaxianApp.hpp"
#include "Game/Events.hpp"
#include <entityx/entityx.h>

using namespace NHTV;
using namespace entityx;

#define ENABLE_DEBUG 1
#define SLOWMO_TD 0.25f

/**
 * @brief Derived application class that wraps up all globals neatly
 */
class GalaxianEditor : public GalaxianApp {
protected:
  virtual bool onCreate(int argc, char **argv) override;
  virtual void onUpdate(float dt) override;
  virtual void onDraw() override;
  virtual void onEditor() override;
  virtual void onDestroy() override;

#if (ENABLE_DEBUG)
  bool m_debbuging = false;
#endif

public:
  void receive(const GameResetEvent &e);
};

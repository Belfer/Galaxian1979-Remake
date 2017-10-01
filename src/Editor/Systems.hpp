#pragma once

#include "Game/Systems.hpp"

/**
 * @brief The DebugSystem class
 */
class DebugSystem : public GalaxianSystem<DebugSystem> {
public:
  DEFINE_SYSTEM(DebugSystem)
  void update(EntityManager &es, EventManager &ev, TimeDelta dt) override;
};

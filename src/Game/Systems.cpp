//#include "Systems.hpp"
//#include "Game/Components.hpp"
//#include "Game/Factory.hpp"
//#include <glm/gtx/compatibility.hpp>
//#include <glm/gtx/projection.hpp>

///**
// * @brief HealthSystem
// */
//void HealthSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  es.each<TransformCmp, HealthCmp>(
//      [&](Entity e, TransformCmp &trx, HealthCmp &hth) {
//        hth.val = hth.val > hth.max ? hth.max : hth.val;
//        if (hth.val <= hth.min) {
//          hth.val = hth.min;
//          ev.emit<EntityRemovedEvent>(e);
//          ev.emit<ExplosionEvent>(trx.pos);
//          e.destroy();
//        };
//      });
//}

///**
// * @brief CameraSystem
// */
//void CameraSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  es.each<TransformCmp, CameraCmp>(
//      [&](Entity e, TransformCmp &trx, CameraCmp &cam) {
//        angle += glm::radians(100 * (float)dt);
//        trx.rot = glm::angleAxis(angle, vec3(0, 0, 1));
//        // cam.camera.position = trx.pos;
//        // cam.camera.rotation = trx.rot;
//      });
//}

///**
// * @brief PhysicsSystem
// */
//void PhysicsSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  es.each<TransformCmp, PhysicsCmp>(
//      [dt](Entity e, TransformCmp &trx, PhysicsCmp &phs) {
//        phs.acc = phs.force / phs.mass;
//        phs.vel += phs.acc;
//        trx.pos += phs.vel * (float)dt;
//      });
//}

///**
// * @brief CollisionSystem
// */
//void CollisionSystem::update(EntityManager &es, EventManager &ev,
//                             TimeDelta dt) {
//  std::vector<Entity> checkList;
//  es.each<TransformCmp, PhysicsCmp>(
//      [&](Entity e, TransformCmp &trx, PhysicsCmp &phs) {
//        checkList.emplace_back(e);
//      });

//  auto i = std::begin(checkList);
//  while (i != std::end(checkList)) {
//    Entity eA = *i;
//    i = checkList.erase(i);

//    if (!eA.valid())
//      continue;

//    for (auto eB : checkList) {
//      if (!eA.valid())
//        break;
//      if (!eB.valid())
//        continue;

//      const auto colA = eA.component<ColliderCmp>();
//      const auto colB = eB.component<ColliderCmp>();

//      if (colA->mask & colB->mask) {
//        const auto trxA = eA.component<TransformCmp>();
//        const auto trxB = eB.component<TransformCmp>();

//        const vec3 tmp = trxB->pos - trxA->pos;
//        const float distSq = tmp.x * tmp.x + tmp.y * tmp.y;
//        const float rSum = colA->radius + colB->radius;

//        // Check for collision
//        if (distSq < rSum * rSum) {
//          ev.emit<CollisionEvent>(std::make_pair(eA, eB));
//        }
//      }
//    }
//  }
//}

///**
// * @brief EntitySpawnerSystem
// */
//void SpawnSystem::receive(const ExplosionEvent &e) {
//  Factory::newParticle(m_entities, m_app, e.pos, ParticleCmp::ONCE, 0,
//                       [](Application &app, ParticleCmp::Particle &p) {
//                         p.pos.first = vec3();
//                         p.pos.second = p.pos.first;
//                         const float th = glm::radians((float)(rand() % 360));
//                         const float c0 = cos(th);
//                         const float s0 = sin(th);
//                         p.vel = vec3(c0 * (100 + (rand() % 900)),
//                                      s0 * (100 + (rand() % 900)), 0);
//                         p.color = 0x00FFFFFF;
//                         p.life = ((rand() % 100) / 100.f) * 0.75f;
//                       },
//                       [](float dt, float maxLife,
//                          std::vector<ParticleCmp::Particle> &particles) {
//                         for (auto &p : particles) {
//                           p.pos.second.x = p.pos.first.x;
//                           p.pos.second.y = p.pos.first.y;
//                           p.pos.first.x += p.vel.x * dt;
//                           p.pos.first.y += p.vel.y * dt;
//                           p.color.a(sqrt(p.life * 2.f) * 255);
//                         }
//                       },
//                       150);
//}

//void SpawnSystem::receive(const EntityRemovedEvent &e) {
//  if (e.entity.valid()) {
//    if (e.entity.has_component<TransformCmp>() &&
//        e.entity.has_component<EnemyCmp>()) {
//      auto trx = e.entity.component<const TransformCmp>();

//      const float r = (rand() % 100) / 100.f;
//      const float p = 0.1f;

//      if (r < p) {
//        Factory::newPowerUp(
//            m_entities, trx->pos, vec3(0, -GVars::powerUpSpeed, 0),
//            GVars::powerShieldSpr, 0xFFFFFFFF, GVars::powerUpRadius,
//            GVars::powerUpMask, PowerCmp::SHIELD);
//      }
//    }
//  }
//}

//void SpawnSystem::receive(const GameResetEvent &e) {
//  m_entities.reset();

//  int sw;
//  int sh;
//  Engine::GetWindow().getWindowSize(sw, sh);

//  auto camEnt = Factory::newCamera(m_entities, vec3(0, 0, 1), quat(), m_app);
//  // auto camera = camEnt.component<CameraCmp>()->camera;
//  // camera.orthographic(0.f, (float)screenWidth, 0.f, (float)screenHeight,
//  // 0.01f, 1000.f);

//  // Spawn bg particles
//  Factory::newParticle(m_entities, m_app, vec3(sw * 0.5f, sh * 0.5f, 0),
//                       ParticleCmp::LOOP, 0,
//                       [](Application &app, ParticleCmp::Particle &p) {
//                         int sw = 0, sh = 0;
//                         Engine::GetWindow().getWindowSize(sw, sh);
//                         p.pos.first = vec3((rand() % sw) - sw * 0.5f, sh, 0);
//                         // std::cout << p.pos.first.x << ", " << p.pos.first.y
//                         // << "\n";
//                         p.pos.second = p.pos.first;
//                         p.vel = vec3(0, -(rand() % 1000), 0);
//                         p.color = 0xFFFFFFFF;
//                         p.life = (rand() % 100) * 0.05f;
//                       },
//                       [](float dt, float maxLife,
//                          std::vector<ParticleCmp::Particle> &particles) {
//                         for (auto &p : particles) {
//                           p.pos.second.x = p.pos.first.x;
//                           p.pos.second.y = p.pos.first.y;
//                           p.pos.first.x += p.vel.x * dt;
//                           p.pos.first.y += p.vel.y * dt;
//                           p.color.a(p.life * 255);
//                         }
//                       },
//                       500);

//  // Spawn player
//  Factory::newPlayer(m_entities, vec3(sw * 0.5f, 64, 0), 3);

//  const float speed[6] = {100, 50, 50, 0, 0, 0};
//  const uint points[6] = {100, 50, 50, 10, 10, 10};
//  const uint colors[6] = {0xFFFFFF3F, 0xFFFFFF6F, 0xFFFFFF6F,
//                          0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

//  // Spawn enemies
//  for (int i = 0; i < 10; ++i) {
//    for (int j = 0; j < 6; ++j) {
//      Factory::newEnemy(m_entities, GVars::enemySpeed + speed[j], points[j],
//                        colors[j], vec3(i, j, 0));
//    }
//  }
//}

///**
// * @brief PlayerSystem
// */
//void PlayerSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  int sw = 0, sh = 0;
//  Engine::GetWindow().getWindowSize(sw, sh);

//  es.each<TransformCmp, PhysicsCmp, HealthCmp, SpriteCmp, PlayerCmp>(
//      [=, &es](Entity e, TransformCmp &trx, PhysicsCmp &phs, HealthCmp &hth,
//               SpriteCmp &spr, PlayerCmp &player) {
//        m_player = e;

//        //    if (m_app.IsKeyDown(KEY_RIGHT)) {
//        //      player.erot.z -= 500.f * dt * DEG2RAD;
//        //    } else if (m_app.IsKeyDown(KEY_LEFT)) {
//        //      player.erot.z += 500.f * dt * DEG2RAD;
//        //    }

//        player.erot.z =
//            player.erot.z > 90 ? 90 : player.erot.z < -90 ? -90 : player.erot.z;
//        // player.erot.z = lerp(player.erot.z, 0.f, 10.f * dt);
//        trx.rot = quat(player.erot);

//        player.forward = trx.rot * vec3(0, 1, 0);
//        phs.vel = proj(player.forward, vec3(1, 0, 0)) * player.speed;

//        player.fireTimer += dt;
//        //    if (m_app.IsKeyDown(KEY_SPACE) && player.fireTimer >=
//        //    player.fireRate) {
//        //      player.fireTimer = 0;
//        //      Factory::newBullet(es, trx.pos + (player.forward * 8.f),
//        //      trx.rot,
//        //                         player.forward * 1000.f, GVars::bulletSpr,
//        //                         0x00FF00FF,
//        //                         GVars::bulletRadius, GVars::playerBulletMask,
//        //                         1);
//        //    }

//        if (trx.pos.x < 0)
//          trx.pos.x = 0;
//        else if (trx.pos.x > sw)
//          trx.pos.x = sw;
//      });
//}

//void PlayerSystem::receive(const EntityRemovedEvent &e) {
//  if (e.entity.has_component<PlayerCmp>()) {
//    m_events.emit<PlayerKilledEvent>();
//  }
//}

//void PlayerSystem::receive(const CollisionEvent &e) {}

//void PlayerSystem::receive(const PowerEnableEvent &e) {
//  m_power = (PowerCmp::PowerUp)(m_power | e.power);
//  if (m_power & PowerCmp::SHIELD) {
//    if (m_player.valid() && m_player.has_component<SpriteCmp>()) {
//      auto spr = m_player.component<SpriteCmp>();
//      spr->handles.emplace_back(GVars::shieldSpr);
//    }
//  }
//}

//void PlayerSystem::receive(const PowerDisableEvent &e) {
//  // m_power = (PowerCmp::PowerUp)(m_power ^ e.power);

//  std::cout << "here\n";
//  if (!(m_power & e.power) && m_player.valid()) {
//    std::cout << "here2\n";
//    if (m_power & PowerCmp::SHIELD) {
//      std::cout << "here3\n";
//      if (m_player.has_component<SpriteCmp>()) {
//        std::cout << "here4\n";
//        auto spr = m_player.component<SpriteCmp>();
//        spr->handles.pop_back();
//      }
//    } else if (m_power & PowerCmp::DOUBLE_SHOT) {
//    }
//  }
//}

///**
// * @brief EnemySystem
// */
//void EnemySystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  int sw = 0, sh = 0;
//  Engine::GetWindow().getWindowSize(sw, sh);

//  uint numEnemies = 0;
//  uint idleEnemies = 0;
//  // Check if outside screen bounds
//  es.each<TransformCmp, EnemyCmp>([=, &numEnemies, &idleEnemies](
//      Entity e, TransformCmp &trx, EnemyCmp &enemy) {
//    numEnemies++;

//    if (enemy.state == EnemyCmp::IDLE) {
//      idleEnemies++;
//      if (trx.pos.x < 0)
//        m_groupDir = 1;
//      else if (trx.pos.x > sw)
//        m_groupDir = -1;

//    } else {
//      if (trx.pos.y < 0 || trx.pos.y > sh) {
//        enemy.state = EnemyCmp::GROUPING;
//        trx.pos.y = sh;
//      }
//    }
//  });

//  if (idleEnemies == 0) {
//    m_groupPos.x = sw * 0.5f;
//    m_groupPos.y = sh * 0.5f;
//  }

//  if (numEnemies > 0) {
//    // Update group pos
//    m_groupPos.x += m_groupSpeed * m_groupDir * dt;

//    Entity player;
//    ComponentHandle<TransformCmp> playerTrx;
//    es.each<PlayerCmp>([&player](Entity e, PlayerCmp &p) { player = e; });
//    if (player.valid()) {
//      if (player.has_component<TransformCmp>())
//        playerTrx = player.component<TransformCmp>();

//      // Update wave spawn variables
//      if (!m_attacking) {
//        m_attackTimer += dt;
//        if (m_attackTimer > 2) {
//          m_attackTimer -= 2;
//          m_attacking = true;
//          m_enemiesAttaking = 1 + (rand() % 9);
//          m_enemyCount = 0;
//        }
//      } else {
//        m_enemyTimer += dt;
//        if (m_enemyTimer > 0.5f) {
//          m_enemyTimer -= 0.5f;

//          bool spawned = false;
//          es.each<TransformCmp, EnemyCmp>(
//              [=, &spawned](Entity e, TransformCmp &trx, EnemyCmp &enemy) {
//                if (!spawned && enemy.state == EnemyCmp::IDLE) {
//                  const float totalEnemies = 60;
//                  const float difficulty = 0.6f;
//                  const float r = (rand() % 100) * 0.01f;
//                  const float p = 1 - (pow(numEnemies, difficulty) /
//                                       pow(totalEnemies, difficulty));
//                  if (r < p) {
//                    enemy.state = EnemyCmp::UNGROUPING;
//                    m_enemyCount++;
//                    spawned = true;
//                  }
//                }
//              });

//          if (m_enemyCount >= m_enemiesAttaking) {
//            m_attacking = false;
//          }
//        }
//      }
//    }

//    // Update enemies
//    es.each<TransformCmp, PhysicsCmp, EnemyCmp>([=, &es](
//        Entity e, TransformCmp &trx, PhysicsCmp &phs, EnemyCmp &enemy) {

//      if (!player.valid() && enemy.state != EnemyCmp::IDLE) {
//        enemy.state = EnemyCmp::GROUPING;
//      }

//      const vec3 origin = vec3((sw * 0.5f) - enemy.idxPos.x * 32,
//                               (sh * 0.5f) - 64 - enemy.idxPos.y * 32, 0);

//      const vec3 dir2Player =
//          playerTrx ? normalize(playerTrx->pos - trx.pos) : vec3();

//      switch (enemy.state) {
//      case EnemyCmp::IDLE:
//        trx.pos = m_groupPos + origin;
//        break;

//      case EnemyCmp::UNGROUPING:
//        enemy.state = EnemyCmp::ATTACKING;
//        break;

//      case EnemyCmp::ATTACKING:
//        enemy.fireTimer += dt;
//        if (enemy.fireTimer >= enemy.fireRate) {
//          enemy.fireTimer = 0;
//          Factory::newBullet(es, trx.pos + (dir2Player * 8.f), trx.rot,
//                             vec3(0, -1000, 0), GVars::bulletSpr, 0x0000FFFF,
//                             GVars::bulletRadius, GVars::enemyBulletMask, 1);
//        }

//        if (trx.pos.y < 256) {
//          enemy.state = EnemyCmp::FLEEING;
//        }

//        phs.vel = dir2Player * enemy.speed;
//        break;

//      case EnemyCmp::FLEEING:
//        phs.vel.y = -enemy.speed;
//        phs.vel.x = -enemy.speed * dir2Player.x;
//        break;

//      case EnemyCmp::GROUPING:
//        phs.vel = normalize((m_groupPos + origin) - trx.pos) * enemy.speed;
//        if (length((m_groupPos + origin) - trx.pos) < 8)
//          enemy.state = EnemyCmp::IDLE;
//        break;
//      }
//    });
//  }
//}

//void EnemySystem::receive(const GameResetEvent &e) {
//  int sw = 0, sh = 0;
//  Engine::GetWindow().getWindowSize(sw, sh);
//  m_groupPos.x = sw * 0.5f;
//  m_groupPos.y = sh * 0.5f;
//}

///**
// * @brief PowerUpSystem
// */
//void PowerUpSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  if (m_powerActive) {
//    if (m_power & PowerCmp::SHIELD) {
//      m_shieldTimer += dt;
//      if (m_shieldTimer >= 5.f) {
//        m_events.emit<PowerDisableEvent>(PowerCmp::SHIELD);
//      }
//    } else if (m_power & PowerCmp::DOUBLE_SHOT) {
//      m_doubleShotTimer += dt;
//      if (m_doubleShotTimer >= 5.f) {
//        m_events.emit<PowerDisableEvent>(PowerCmp::DOUBLE_SHOT);
//      }
//    }
//  }

//  es.each<HealthCmp, SpriteCmp, PlayerCmp>(
//      [=](Entity e, HealthCmp &hth, SpriteCmp &spr, PlayerCmp &player) {
//        if (m_powerActive) {
//          if (m_power & PowerCmp::SHIELD) {
//            hth.val = hth.max;
//          }
//        }
//      });
//}

//void PowerUpSystem::receive(const GameResetEvent &e) {
//  m_powerActive = false;
//  m_shieldTimer = 0;
//  m_power = PowerCmp::NONE;
//}

//void PowerUpSystem::receive(const CollisionEvent &e) {
//  if (e.collision.first.valid() &&
//      e.collision.first.has_component<PowerCmp>()) {
//    auto power = e.collision.first.component<PowerCmp>();
//    m_events.emit<PowerEnableEvent>(power->power);
//    e.collision.first.destroy();
//  }

//  if (e.collision.second.valid() &&
//      e.collision.second.has_component<PowerCmp>()) {
//    auto power = e.collision.second.component<PowerCmp>();
//    m_events.emit<PowerEnableEvent>(power->power);
//    e.collision.second.destroy();
//  }
//}

//void PowerUpSystem::receive(const PowerEnableEvent &e) {
//  m_power = (PowerCmp::PowerUp)(m_power | e.power);
//  m_powerActive = true;

//  if (e.power & PowerCmp::SHIELD) {
//    m_shieldTimer = 0.f;
//  } else if (e.power & PowerCmp::DOUBLE_SHOT) {
//    m_doubleShotTimer = 0.f;
//  }
//}

//void PowerUpSystem::receive(const PowerDisableEvent &e) {
//  if (e.power & PowerCmp::SHIELD) {
//    m_shieldTimer = 0.f;
//  }

//  m_power = PowerCmp::NONE;
//  m_powerActive = false;
//}

///**
// * @brief BulletSystem
// */
//void BulletSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  es.each<TransformCmp, BulletCmp>(
//      [=](Entity e, TransformCmp &trx, BulletCmp &bullet) {
//        int sw = 0, sh = 0;
//        Engine::GetWindow().getWindowSize(sw, sh);

//        // Remove if outside bounds
//        if (trx.pos.y < 0 || trx.pos.y > sh)
//          e.destroy();
//      });
//}

//void BulletSystem::receive(const CollisionEvent &e) {

//  if (e.collision.first.valid() &&
//      e.collision.first.has_component<BulletCmp>()) {
//    if (e.collision.second.valid() &&
//        e.collision.second.has_component<HealthCmp>()) {

//      auto bullet = e.collision.first.component<BulletCmp>();
//      auto health = e.collision.second.component<HealthCmp>();
//      health->val -= bullet->damage;
//    }
//    e.collision.first.destroy();
//  }

//  if (e.collision.second.valid() &&
//      e.collision.second.has_component<BulletCmp>()) {
//    if (e.collision.first.valid() &&
//        e.collision.first.has_component<HealthCmp>()) {

//      auto bullet = e.collision.second.component<BulletCmp>();
//      auto health = e.collision.first.component<HealthCmp>();
//      health->val -= bullet->damage;
//    }
//    e.collision.second.destroy();
//  }
//}

///**
// * @brief MenuSystem
// */
//void MenuSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  int sw = 0, sh = 0;
//  Engine::GetWindow().getWindowSize(sw, sh);

//  // m_app.SetFont("../resources/fonts/invaders.json.fnt");
//  // m_app.DrawString("1UP", (screenWidth * 0.01f), screenHeight - 5, 1);

//  // m_app.DrawString(std::to_string(m_score).c_str(), (screenWidth * 0.01f),
//  //                 screenHeight - 30, 1);

//  // m_app.DrawString("HIGH SCORE", (screenWidth * 0.9f) - 100, screenHeight -
//  // 5,1);
//  // m_app.DrawString(std::to_string(m_highscore).c_str(), (screenWidth * 0.9f)
//  // - 100, screenHeight - 30, 1);

//  // m_app.DrawLine(0, 40, screenWidth, 40, 0xFCFCFCFF);
//  // m_app.DrawString("CREDITS: 0", (screenWidth * 0.9f) - 100,
//  // 38);

//  // m_app.SetFont(nullptr);
//}

//void MenuSystem::receive(const GameResetEvent &e) { m_score = 0; }

//void MenuSystem::receive(const EntityRemovedEvent &e) {
//  if (e.entity.has_component<EnemyCmp>()) {
//    auto enemy = e.entity.component<const EnemyCmp>();
//    m_score += enemy->points;
//    if (m_score > m_highscore)
//      m_highscore = m_score;
//  }
//}

///**
// * @brief ParticleSystem
// */
//void ParticleSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  es.each<TransformCmp, ParticleCmp>(
//      [=](Entity e, TransformCmp &trx, ParticleCmp &psc) {
//        psc.updateFn(dt, psc.maxLife, psc.particles);

//        vec3 pA, pB;
//        bool allDead = true;
//        for (auto &p : psc.particles) {
//          p.life -= dt;
//          if (p.life <= 0) {
//            if (psc.mode == ParticleCmp::LOOP)
//              psc.resetFn(m_app, p);
//          } else {
//            allDead = false;
//          }

//          pA.x = p.pos.first.x / p.zdepth;
//          pA.y = p.pos.first.y / p.zdepth;

//          pB.x = p.pos.second.x / p.zdepth;
//          pB.y = p.pos.second.y / p.zdepth;

//          // m_app.DrawLine(trx.pos.x + pA.x, trx.pos.y + pA.y, trx.pos.x +
//          // pB.x, trx.pos.y + pB.y, p.color);
//        }

//        if (allDead && psc.mode == ParticleCmp::ONCE) {
//          e.destroy();
//        }
//      });
//}

///**
// * @brief SpriteSystem
// */
//void SpriteSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  //  es.each<TransformCmp, SpriteCmp>(
//  //      [=](Entity e, TransformCmp &trx, SpriteCmp &spr) {
//  //        for (uint handle : spr.handles) {
//  //          m_app.RotateSprite(handle, trx.rot);
//  //          m_app.MoveSprite(handle, trx.pos.x, trx.pos.y);
//  //          m_app.SetSpriteColour(handle, spr.color);
//  //          m_app.DrawSprite(handle);
//  //        }
//  //      });
//}

///**
// * @brief TextSystem
// */
//void TextSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  //  es.each<TransformCmp, TextCmp>(
//  //      [=](Entity e, TransformCmp &trx, TextCmp &txt) {
//  //        m_app.DrawString(txt.textStr.c_str(), trx.pos.x, trx.pos.y,
//  //                         txt.fontSize, txt.color);
//  //      });
//}

///**
// * @brief LineSystem
// */
//void LineSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
//  //  es.each<TransformCmp, LineCmp>(
//  //      [=](Entity e, TransformCmp &trx, LineCmp &lin) {
//  //        m_app.DrawLine(trx.pos.x + lin.pA.x, trx.pos.y + lin.pA.y,
//  //                       trx.pos.x + lin.pB.x, trx.pos.y + lin.pB.y,
//  //                       lin.color);
//  //      });
//}

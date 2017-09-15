#include "Systems.hpp"
#include "Components.hpp"
#include "Factory.hpp"

/**
 * @brief HealthSystem
 */
void HealthSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, HealthCmp>(
      [&](Entity e, TransformCmp &trx, HealthCmp &hth) {
        hth.val = hth.val > hth.max ? hth.max : hth.val;
        if (hth.val <= hth.min) {
          hth.val = hth.min;
          ev.emit<EntityRemovedEvent>(e);
          ev.emit<ExplosionEvent>(trx.pos);
          e.destroy();
        };
      });
}

/**
 * @brief PhysicsSystem
 */
void PhysicsSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, PhysicsCmp>(
      [dt](Entity e, TransformCmp &trx, PhysicsCmp &phs) {
        phs.acc = phs.force / phs.mass;
        phs.vel += phs.acc;
        trx.pos += phs.vel * dt;
      });
}

/**
 * @brief CollisionSystem
 */
void CollisionSystem::update(EntityManager &es, EventManager &ev,
                             TimeDelta dt) {
  std::vector<Entity> checkList;
  es.each<TransformCmp, PhysicsCmp>(
      [&](Entity e, TransformCmp &trx, PhysicsCmp &phs) {
        checkList.emplace_back(e);
      });

  auto i = std::begin(checkList);
  while (i != std::end(checkList)) {
    Entity eA = *i;
    i = checkList.erase(i);

    if (!eA.valid())
      continue;

    for (auto eB : checkList) {
      if (!eA.valid())
        break;
      if (!eB.valid())
        continue;

      const auto trxA = eA.component<TransformCmp>();
      const auto trxB = eB.component<TransformCmp>();
      const auto colA = eA.component<ColliderCmp>();
      const auto colB = eB.component<ColliderCmp>();

      const vec2 tmp = trxB->pos - trxA->pos;
      const float distSq = tmp.x * tmp.x + tmp.y * tmp.y;
      const float rSum = colA->radius + colB->radius;

      // Check for collision
      if (distSq < rSum * rSum) {
        ev.emit<CollisionEvent>(std::make_pair(eA, eB));
      }
    }
  }
}

/**
 * @brief EntitySpawnerSystem
 */
void EntitySpawnerSystem::update(EntityManager &es, EventManager &ev,
                                 TimeDelta dt) {}

void EntitySpawnerSystem::receive(const ExplosionEvent &e) {
  Factory::newParticle(m_entities, e.pos, ParticleCmp::ONCE, 0,
                       [](ParticleCmp::Particle &p) {
                         p.pos.first = vec2();
                         p.pos.second = p.pos.first;
                         const float th = (rand() % 360) * DEG2RAD;
                         const float c0 = cos(th);
                         const float s0 = sin(th);
                         p.vel = vec2(c0 * (100 + (rand() % 900)),
                                      s0 * (100 + (rand() % 900)));
                         p.color = 0x00FFFFFF;
                         p.life = ((rand() % 100) / 100.f) * 0.75f;
                       },
                       [](float dt, float maxLife,
                          std::vector<ParticleCmp::Particle> &particles) {
                         for (auto &p : particles) {
                           p.pos.second.x = p.pos.first.x;
                           p.pos.second.y = p.pos.first.y;
                           p.pos.first.x += p.vel.x * dt;
                           p.pos.first.y += p.vel.y * dt;
                           p.color.argb.colours.a = sqrt(p.life * 2.f) * 255;
                         }
                       },
                       150);
}

void EntitySpawnerSystem::receive(const GameResetEvent &e) {
  m_entities.reset();

  int screenWidth;
  int screenHeight;
  m_app.GetScreenSize(screenWidth, screenHeight);

  // Spawn bg particles
  Factory::newParticle(m_entities,
                       vec2(screenWidth * 0.5f, screenHeight * 0.5f),
                       ParticleCmp::LOOP, 0,
                       [](ParticleCmp::Particle &p) {
                         p.pos.first = vec2((rand() % 512) - 256, 768);
                         p.pos.second = p.pos.first;
                         p.vel = vec2(0, -(rand() % 1000));
                         p.color = 0xFFFFFFFF;
                         p.life = (rand() % 100) * 0.05f;
                       },
                       [](float dt, float maxLife,
                          std::vector<ParticleCmp::Particle> &particles) {
                         for (auto &p : particles) {
                           p.pos.second.x = p.pos.first.x;
                           p.pos.second.y = p.pos.first.y;
                           p.pos.first.x += p.vel.x * dt;
                           p.pos.first.y += p.vel.y * dt;
                           p.color.argb.colours.a = p.life * 255;
                         }
                       },
                       500);

  // Spawn player
  Factory::newPlayer(m_entities, vec2(screenWidth * 0.5f, 64), m_vars.playerSpr,
                     14, m_vars.bulletSpr, m_vars.playerFirerate,
                     m_vars.playerSpeed, 3);

  const float speed[6] = {100, 50, 50, 0, 0, 0};
  const unsigned int points[6] = {100, 50, 50, 10, 10, 10};
  const unsigned int colors[6] = {0xFFFFFF3F, 0xFFFFFF6F, 0xFFFFFF6F,
                                  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

  // Spawn enemies
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 6; ++j) {
      Factory::newEnemy(m_entities, m_vars.enemySpr, 14, m_vars.bulletSpr,
                        m_vars.enemyFirerate, m_vars.enemySpeed + speed[j],
                        points[j], colors[j], vec2(i, j));
    }
  }
}

/**
 * @brief PlayerSystem
 */
void PlayerSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  int screenWidth;
  int screenHeight;
  m_app.GetScreenSize(screenWidth, screenHeight);

  es.each<TransformCmp, PhysicsCmp, PlayerCmp>([=, &es](
      Entity e, TransformCmp &trx, PhysicsCmp &phs, PlayerCmp &player) {

    if (m_app.IsKeyDown(KEY_RIGHT)) {
      trx.rot -= 500.f * dt;
    } else if (m_app.IsKeyDown(KEY_LEFT)) {
      trx.rot += 500.f * dt;
    }

    trx.rot = trx.rot > 90 ? 90 : trx.rot < -90 ? -90 : trx.rot;
    trx.rot = Lerp(trx.rot, 0, 10.f * dt);

    player.dir.x = cos((trx.rot + 90) * DEG2RAD);
    player.dir.y = sin((trx.rot + 90) * DEG2RAD);

    phs.vel = proj(player.dir, vec2(1, 0)) * player.speed;

    player.fireTimer += dt;
    if (m_app.IsKeyDown(KEY_SPACE) && player.fireTimer >= player.fireRate) {
      player.fireTimer = 0;
      Factory::newBullet(es, trx.pos + (player.dir * 32), player.dir * 1000,
                         player.bulletSpr, 0x00FF00FF, 6, 1);
    }

    if (trx.pos.x < 0)
      trx.pos.x = 0;
    else if (trx.pos.x > screenWidth)
      trx.pos.x = screenWidth;
  });
}

void PlayerSystem::receive(const EntityRemovedEvent &e) {
  if (e.entity.has_component<PlayerCmp>()) {
    m_events.emit<PlayerKilledEvent>();
  }
}

/**
 * @brief EnemySystem
 */
void EnemySystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  int screenWidth;
  int screenHeight;
  m_app.GetScreenSize(screenWidth, screenHeight);

  unsigned int numEnemies = 0;
  unsigned int idleEnemies = 0;
  // Check if outside screen bounds
  es.each<TransformCmp, EnemyCmp>([=, &numEnemies, &idleEnemies](
      Entity e, TransformCmp &trx, EnemyCmp &enemy) {
    numEnemies++;

    if (enemy.state == EnemyCmp::IDLE) {
      idleEnemies++;
      if (trx.pos.x < 0)
        m_groupDir = 1;
      else if (trx.pos.x > screenWidth)
        m_groupDir = -1;

    } else {
      if (trx.pos.y < 0 || trx.pos.y > screenHeight) {
        enemy.state = EnemyCmp::GROUPING;
        trx.pos.y = screenHeight;
      }
    }
  });

  if (idleEnemies == 0) {
    m_groupPos.x = screenWidth * 0.5f;
    m_groupPos.y = screenHeight * 0.5f;
  }

  if (numEnemies > 0) {
    // Update group pos
    m_groupPos.x += m_groupSpeed * m_groupDir * dt;

    Entity player;
    ComponentHandle<TransformCmp> playerTrx;
    es.each<PlayerCmp>([&player](Entity e, PlayerCmp &p) { player = e; });
    if (player.valid()) {
      if (player.has_component<TransformCmp>())
        playerTrx = player.component<TransformCmp>();

      // Update wave spawn variables
      if (!m_attacking) {
        m_attackTimer += dt;
        if (m_attackTimer > 2) {
          m_attackTimer -= 2;
          m_attacking = true;
          m_enemiesAttaking = 1 + (rand() % 9);
          m_enemyCount = 0;
        }
      } else {
        m_enemyTimer += dt;
        if (m_enemyTimer > 0.5f) {
          m_enemyTimer -= 0.5f;

          bool spawned = false;
          es.each<TransformCmp, EnemyCmp>(
              [=, &spawned](Entity e, TransformCmp &trx, EnemyCmp &enemy) {
                if (!spawned && enemy.state == EnemyCmp::IDLE) {
                  const float totalEnemies = 60;
                  const float difficulty = 0.6f;
                  const float r = (rand() % 100) * 0.01f;
                  const float p = 1 - (pow(numEnemies, difficulty) /
                                       pow(totalEnemies, difficulty));
                  if (r < p) {
                    enemy.state = EnemyCmp::UNGROUPING;
                    m_enemyCount++;
                    spawned = true;
                  }
                }
              });

          if (m_enemyCount >= m_enemiesAttaking) {
            m_attacking = false;
          }
        }
      }
    }

    // Update enemies
    es.each<TransformCmp, PhysicsCmp, EnemyCmp>([=, &es](
        Entity e, TransformCmp &trx, PhysicsCmp &phs, EnemyCmp &enemy) {

      if (!player.valid() && enemy.state != EnemyCmp::IDLE) {
        enemy.state = EnemyCmp::GROUPING;
      }

      const vec2 origin =
          vec2((screenWidth * 0.5f) - enemy.idxPos.x * 32,
               (screenHeight * 0.5f) - 64 - enemy.idxPos.y * 32);

      const vec2 dir2Player =
          playerTrx ? normalize(playerTrx->pos - trx.pos) : vec2();

      switch (enemy.state) {
      case EnemyCmp::IDLE:
        trx.pos = m_groupPos + origin;
        break;

      case EnemyCmp::UNGROUPING:
        enemy.state = EnemyCmp::ATTACKING;
        break;

      case EnemyCmp::ATTACKING:
        enemy.fireTimer += dt;
        if (enemy.fireTimer >= enemy.fireRate) {
          enemy.fireTimer = 0;
          Factory::newBullet(es, trx.pos + (dir2Player * 32), vec2(0, -1000),
                             enemy.bulletSpr, 0x0000FFFF, 6, 1);
        }

        if (trx.pos.y < 256) {
          enemy.state = EnemyCmp::FLEEING;
        }

        phs.vel = dir2Player * enemy.speed;
        break;

      case EnemyCmp::FLEEING:
        phs.vel.y = -enemy.speed;
        phs.vel.x = -enemy.speed * dir2Player.x;
        break;

      case EnemyCmp::GROUPING:
        phs.vel = normalize((m_groupPos + origin) - trx.pos) * enemy.speed;
        if (length((m_groupPos + origin) - trx.pos) < 8)
          enemy.state = EnemyCmp::IDLE;
        break;
      }
    });
  }
}

void EnemySystem::receive(const GameResetEvent &e) {
  int screenWidth;
  int screenHeight;
  m_app.GetScreenSize(screenWidth, screenHeight);
  m_groupPos.x = screenWidth * 0.5f;
  m_groupPos.y = screenHeight * 0.5f;
}

/**
 * @brief BulletSystem
 */
void BulletSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, BulletCmp>(
      [=](Entity e, TransformCmp &trx, BulletCmp &bullet) {
        int screenWidth;
        int screenHeight;
        m_app.GetScreenSize(screenWidth, screenHeight);

        // Remove if outside bounds
        if (trx.pos.y < 0 || trx.pos.y > screenHeight)
          e.destroy();
      });
}

void BulletSystem::receive(const CollisionEvent &e) {

  if (e.collision.first.valid() &&
      e.collision.first.has_component<BulletCmp>()) {
    if (e.collision.second.valid() &&
        e.collision.second.has_component<HealthCmp>()) {

      auto bullet = e.collision.first.component<BulletCmp>();
      auto health = e.collision.second.component<HealthCmp>();
      health->val -= bullet->damage;
    }
    e.collision.first.destroy();
  }

  if (e.collision.second.valid() &&
      e.collision.second.has_component<BulletCmp>()) {
    if (e.collision.first.valid() &&
        e.collision.first.has_component<HealthCmp>()) {

      auto bullet = e.collision.second.component<BulletCmp>();
      auto health = e.collision.first.component<HealthCmp>();
      health->val -= bullet->damage;
    }
    e.collision.second.destroy();
  }
}

/**
 * @brief MenuSystem
 */
void MenuSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  int screenWidth;
  int screenHeight;
  m_app.GetScreenSize(screenWidth, screenHeight);

  m_app.SetFont("../resources/fonts/invaders.fnt");
  m_app.DrawString("1UP", (screenWidth * 0.01f), screenHeight - 5, 1);

  m_app.DrawString(std::to_string(m_score).c_str(), (screenWidth * 0.01f),
                   screenHeight - 30, 1);

  m_app.DrawString("HIGH SCORE", (screenWidth * 0.9f) - 100, screenHeight - 5,
                   1);
  m_app.DrawString(std::to_string(m_highscore).c_str(),
                   (screenWidth * 0.9f) - 100, screenHeight - 30, 1);

  m_app.DrawLine(0, 40, screenWidth, 40, 0xFCFCFCFF);
  m_app.DrawString("CREDITS: 0", (screenWidth * 0.9f) - 100, 38);

  m_app.SetFont(nullptr);
}

void MenuSystem::receive(const GameResetEvent &e) { m_score = 0; }

void MenuSystem::receive(const EntityRemovedEvent &e) {
  if (e.entity.has_component<EnemyCmp>()) {
    auto enemy = e.entity.component<const EnemyCmp>();
    m_score += enemy->points;
    if (m_score > m_highscore)
      m_highscore = m_score;
  }
}

/**
 * @brief ParticleSystem
 */
void ParticleSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, ParticleCmp>(
      [=](Entity e, TransformCmp &trx, ParticleCmp &psc) {
        psc.updateFn(dt, psc.maxLife, psc.particles);

        vec2 pA, pB;
        bool allDead = true;
        for (auto &p : psc.particles) {
          p.life -= dt;
          if (p.life <= 0) {
            if (psc.mode == ParticleCmp::LOOP)
              psc.resetFn(p);
          } else {
            allDead = false;
          }

          pA.x = p.pos.first.x / p.zdepth;
          pA.y = p.pos.first.y / p.zdepth;

          pB.x = p.pos.second.x / p.zdepth;
          pB.y = p.pos.second.y / p.zdepth;

          m_app.DrawLine(trx.pos.x + pA.x, trx.pos.y + pA.y, trx.pos.x + pB.x,
                         trx.pos.y + pB.y, p.color);
        }

        if (allDead && psc.mode == ParticleCmp::ONCE) {
          e.destroy();
        }
      });
}

/**
 * @brief SpriteSystem
 */
void SpriteSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, SpriteCmp>(
      [=](Entity e, TransformCmp &trx, SpriteCmp &spr) {
        m_app.RotateSprite(spr.handle, trx.rot);
        m_app.MoveSprite(spr.handle, trx.pos.x, trx.pos.y);
        m_app.SetSpriteColour(spr.handle, spr.color);
        m_app.DrawSprite(spr.handle);
      });
}

/**
 * @brief TextSystem
 */
void TextSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, TextCmp>(
      [=](Entity e, TransformCmp &trx, TextCmp &txt) {
        m_app.DrawString(txt.textStr.c_str(), trx.pos.x, trx.pos.y,
                         txt.fontSize, txt.color);
      });
}

/**
 * @brief LineSystem
 */
void LineSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, LineCmp>(
      [=](Entity e, TransformCmp &trx, LineCmp &lin) {
        m_app.DrawLine(trx.pos.x + lin.pA.x, trx.pos.y + lin.pA.y,
                       trx.pos.x + lin.pB.x, trx.pos.y + lin.pB.y, lin.color);
      });
}

/**
 * @brief DebugSystem
 */
void DebugSystem::update(EntityManager &es, EventManager &ev, TimeDelta dt) {
  es.each<TransformCmp, PhysicsCmp, ColliderCmp>(
      [=](Entity e, TransformCmp &trx, PhysicsCmp &phs, ColliderCmp &col) {
        // Draws velocity vector (blue)
        m_app.DrawLine(trx.pos.x, trx.pos.y, trx.pos.x + phs.vel.x * dt,
                       trx.pos.y + phs.vel.y * dt, 0xFF0000FF, 0xFF0000FF);

        // Draws approx of circle collider with two diagonals (green)
        m_app.DrawLine(trx.pos.x - col.radius, trx.pos.y - col.radius,
                       trx.pos.x + col.radius, trx.pos.y + col.radius,
                       0x00FF00FF, 0x00FF00FF);
        m_app.DrawLine(trx.pos.x + col.radius, trx.pos.y - col.radius,
                       trx.pos.x - col.radius, trx.pos.y + col.radius,
                       0x00FF00FF, 0x00FF00FF);
      });

  // Calculate, update and draw FPS
  m_fps++;
  m_fpsTimer += m_app.GetDeltaTime();
  if (m_fpsTimer >= 1.f) {
    m_fpsTimer -= 1.f;
    m_fpsStr = std::to_string(m_fps);
    m_fps = 0;
  }
  m_app.SetFont("../resources/fonts/invaders.fnt");
  m_app.DrawString(("FPS: " + m_fpsStr).c_str(), 5, 20, 0.5f);
  m_app.SetFont(nullptr);
}

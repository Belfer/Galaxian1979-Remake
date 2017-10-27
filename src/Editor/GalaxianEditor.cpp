#include "GalaxianEditor.hpp"
#include "Core/Color.hpp"
#include "Core/Global.hpp"
#include "Core/LineBatch.hpp"
#include "Core/SpriteBatch.hpp"
#include "Game/Components.hpp"
#include "imgui_impl.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>
#include <iostream>

struct Ent {
  vec4 col = vec4();
  vec2 pos = vec2();
  vec2 vel = vec2();
  float rot = 0;
  float trq = 0;
  float scl = 1;
};

#define NUM_ENTS 10000 // 4000
Ent ents[NUM_ENTS];

using namespace glm;

LineBatch *pLineBatch;
SpriteBatch *pSpriteBatch;

Entity entity;

Entity newEntity(EntityManager &es, const std::string &name,
                 const std::string &tag, const vec3 &pos, float rot, float scl,
                 const vec3 &vel, float trq, size_t texture, size_t shader,
                 const vec2 &origin, const vec2 &size, const vec4 &uv) {
  auto e = es.create();
  e.assign<EntityCmp>(name, tag);
  e.assign<TransformCmp>(pos, vec3(scl), angleAxis(rot, vec3(0, 0, 1)));
  e.assign<PhysicsCmp>(vel, trq, 1.f);
  e.assign<MaterialCmp>(texture, shader);
  e.assign<SpriteCmp>(origin, size, uv);
  return e;
}

bool GalaxianEditor::init(int argc, char **args) {
  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(Engine::GetWindow().getPtr(), true);

  int w = 0, h = 0;
  Engine::GetWindow().getWindowSize(w, h);

  // Setup camera
  size_t cameraId = Engine::GetRenderer().newCamera();
  m_pEditorCam = &Engine::GetRenderer().getCamera(cameraId);
  m_pEditorCam->setViewSize(w, h);
  Engine::GetRenderer().bindCamera(cameraId);

  // Setup texture
  m_sprTexture = Engine::GetRenderer().newTexture();
  Texture &sprTexture = Engine::GetRenderer().getTexture(m_sprTexture);

  sprTexture.load(Global::ResPath + "/images/invader.png", Texture::RGBA);
  sprTexture.setMipmapFilter(Texture::NEAREST_MIPMAP_NEAREST);
  sprTexture.setSampleFilter(Texture::NEAREST);

  // Setup shader
  m_sprShader = Engine::GetRenderer().newShader();
  Shader &sprShader = Engine::GetRenderer().getShader(m_sprShader);

  sprShader.load(Global::ResPath + "/shaders/sprite.vert",
                 Global::ResPath + "/shaders/sprite.frag");

  m_lineShader = Engine::GetRenderer().newShader();
  Shader &linShader = Engine::GetRenderer().getShader(m_lineShader);

  linShader.load(Global::ResPath + "/shaders/line.vert",
                 Global::ResPath + "/shaders/line.frag");

  // Setup line batch
  pLineBatch = new LineBatch(Engine::GetRenderer());
  pLineBatch->configure();

  // Setup sprite batch
  pSpriteBatch = new SpriteBatch(Engine::GetRenderer(), NUM_ENTS);
  pSpriteBatch->configure();

  for (auto &ent : ents) {
    ent.col = vec4((rand() % 100) / 100.f, (rand() % 100) / 100.f,
                   (rand() % 100) / 100.f, (rand() % 100) / 100.f);
    ent.pos = vec2((rand() % 768) - 384, (rand() % 768) - 384);
    ent.vel = vec2(cos(rand() % 360) * 100, sin(rand() % 360) * 100);
    ent.trq = (float)(rand() % 10);
    ent.scl = 1 + (rand() % 5) / 5.f;
  }

  entity = newEntity(entities, "name", "tag", vec3(0, 0, 0), 0, 0,
                     vec3(0, 0, 0), 0, m_sprTexture, m_sprShader, vec2(0, 0),
                     vec2(32, 32), vec4(0, 0, 32, 32));

  // Add systems
  //  const size_t numSys = 15;
  //  m_profiles.reserve(numSys);

  //  regProfileSys<HealthSystem>("HealthSystem");
  //  regProfileSys<CameraSystem>("CameraSystem");
  //  regProfileSys<PhysicsSystem>("PhysicsSystem");
  //  regProfileSys<CollisionSystem>("CollisionSystem");
  //  regProfileSys<PlayerSystem>("PlayerSystem");
  //  regProfileSys<EnemySystem>("EnemySystem");
  //  regProfileSys<BulletSystem>("BulletSystem");
  //  regProfileSys<PowerUpSystem>("PowerUpSystem");
  //  regProfileSys<SpawnSystem>("SpawnSystem");

  //  regProfileSys<MenuSystem>("MenuSystem");
  //  regProfileSys<ParticleSystem>("ParticleSystem");
  //  regProfileSys<SpriteSystem>("SpriteSystem");
  //  regProfileSys<TextSystem>("TextSystem");
  //  regProfileSys<LineSystem>("LineSystem");
  //  regProfileSys<DebugSystem>("DebugSystem");
  //  systems.configure();

  return true;
}

void GalaxianEditor::fixed(float dt) {
  // std::cout << dt << "f\n";
}

const vec4 spriteRct(-16, -16, 16, 16);
const vec4 spriteUVs(0, 0, 1, 1);
mat4x4 modelview;
float t = 0;

void GalaxianEditor::pre_update(float dt) {}

void GalaxianEditor::update(float dt) {
  // std::cout << dt << "u\n\n";
  t += dt;

  int w = 0, h = 0;
  Engine::GetWindow().getWindowSize(w, h);
  m_pEditorCam->update(w, h);

  double mx = 0, my = 0;
  // Engine::GetWindow().getMousePos(mx, my);
  vec4 mp(mx, my, 0, 1);
  // mp.x -= w * 0.5f;1
  // mp.y = (h - mp.y) - h * 0.5f;
  // mp = m_editorCam.getView() * mp;

  pLineBatch->clear();
  pLineBatch->drawLine(vec3(mp.x - 4, mp.y - 4, -5),
                       vec3(mp.x + 4, mp.y + 4, -5), vec4(1, 1, 1, 1),
                       vec4(1, 1, 1, 1));
  pLineBatch->drawLine(vec3(mp.x - 4, mp.y + 4, -5),
                       vec3(mp.x + 4, mp.y - 4, -5), vec4(1, 1, 1, 1),
                       vec4(1, 1, 1, 1));
  pLineBatch->update();

  // float p = glfwGetTime();
  pSpriteBatch->clear();
  for (auto &ent : ents) {
    ent.pos += ent.vel * dt;
    ent.rot += ent.trq * dt;

    if (ent.pos.x < -384)
      ent.pos.x = 384;
    else if (ent.pos.x > 384)
      ent.pos.x = -384;
    if (ent.pos.y < -384)
      ent.pos.y = 384;
    else if (ent.pos.y > 384)
      ent.pos.y = -384;

    modelview = glm::translate(vec3(ent.pos.x, ent.pos.y, -5)) *
                glm::rotate(ent.rot, vec3(0.f, 0.f, 1.f)) *
                glm::scale(vec3(ent.scl));
    pSpriteBatch->drawSprite(spriteRct, spriteUVs, ent.col, modelview);
  }
  pSpriteBatch->update();
  //  // std::cout << (glfwGetTime() - p) << "\n";
  // std::cout << (100 * (glfwGetTime() - p) / (1.f / 60.f)) << "\n\n";

  // Calculate FPS
  m_fps++;
  m_fpsTimer += dt;
  if (m_fpsTimer >= 1.f) {
    //    for (auto &p : m_profiles) {
    //      p.frameTimeStr = std::to_string(p.frameTime * 60 * 100);
    //    }

    m_fpsTimer -= 1.f;
    m_fpsStr = std::to_string(m_fps);
    m_fps = 0;
  }

  // Update and profile logic systems
  //  updateProfileSys<HealthSystem>(0, dt);
  //  updateProfileSys<CameraSystem>(1, dt);
  //  updateProfileSys<PhysicsSystem>(2, dt);
  //  updateProfileSys<CollisionSystem>(3, dt);
  //  updateProfileSys<PlayerSystem>(4, dt);
  //  updateProfileSys<EnemySystem>(5, dt);
  //  updateProfileSys<BulletSystem>(6, dt);
  //  updateProfileSys<PowerUpSystem>(7, dt);
  //  updateProfileSys<SpawnSystem>(8, dt);
}

void GalaxianEditor::post_update(float dt) {}

void GalaxianEditor::pre_draw(float dt) {}

void GalaxianEditor::draw(Camera &camera, float dt) {
  Engine::GetRenderer().bindTexture(m_sprTexture);
  Engine::GetRenderer().bindShader(m_sprShader);
  Engine::GetRenderer().draw(pSpriteBatch);
  // Engine::GetRenderer().draw(pLineBatch);

  // Update and profile render systems
  //  updateProfileSys<MenuSystem>(9, dt);
  //  updateProfileSys<ParticleSystem>(10, dt);
  //  updateProfileSys<SpriteSystem>(11, dt);
  //  updateProfileSys<TextSystem>(12, dt);
  //  updateProfileSys<LineSystem>(13, dt);
  //  if (m_debbuging)
  //    updateProfileSys<DebugSystem>(14, dt);
}

void GalaxianEditor::post_draw(float dt) {}

void GalaxianEditor::editor() {
  // draw(m_pEditorCam, 0);

  ImGui_ImplGlfwGL3_NewFrame();

  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("File")) {
    ImGui::MenuItem("New Scene", "", false, true);
    ImGui::MenuItem("Load Scene", "", false, true);
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Edit")) {
    ImGui::MenuItem("Undo", "", false, true);
    ImGui::MenuItem("Redo", "", false, true);
    ImGui::MenuItem("Cut", "", false, true);
    ImGui::MenuItem("Copy", "", false, true);
    ImGui::MenuItem("Paste", "", false, true);
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("View")) {
    ImGui::Checkbox("Profiler", &m_profiler);
    ImGui::Checkbox("Editor", &m_editor);
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Settings")) {
    ImGui::Text("- PolyMode -");
    if (ImGui::RadioButton("Filled", &m_polymode, 0)) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_BLEND);
    }
    if (ImGui::RadioButton("Wired", &m_polymode, 1)) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_BLEND);
    }
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();

  if (m_profiler) {
    ImGui::Begin("- Profiler -");
    ImGui::Text("- Stats -");
    ImGui::Text(("FPS: " + m_fpsStr).c_str());
    for (auto p : m_profiles) {
      ImGui::Text((p.name + ": " + p.frameTimeStr).c_str());
    }
    ImGui::End();
  }

  if (m_editor) {
    ImGui::Begin("- Editor -");
    ImGui::End();
  }

  ImGui::Begin("- Inspector -");

  if (entity.has_component<TransformCmp>()) {
    const auto &trx = entity.component<TransformCmp>();
    ImGui::Text("- Transform -");
    ImGui::InputFloat3("Position",
                       const_cast<float *>(glm::value_ptr(trx->pos)), 3);
    ImGui::InputFloat3("Scale", const_cast<float *>(glm::value_ptr(trx->scl)),
                       3);
    ImGui::InputFloat4("Rotation",
                       const_cast<float *>(glm::value_ptr(trx->rot)), 3);
  }

  if (entity.has_component<PhysicsCmp>()) {
    const auto &phy = entity.component<PhysicsCmp>();
    ImGui::Text("- Physics -");
    ImGui::InputFloat("Mass", const_cast<float *>(&phy->mass), 0.1f, 1.0f, 3);
    ImGui::InputFloat("Troque", const_cast<float *>(&phy->trq), 0.1f, 1.0f, 3);
    ImGui::InputFloat3("Velocity",
                       const_cast<float *>(glm::value_ptr(phy->vel)), 3);
    //    ImGui::InputFloat3("Acceleration",
    //                       const_cast<float *>(glm::value_ptr(phy->acc)), 3);
    //    ImGui::InputFloat3("Force", const_cast<float
    //    *>(glm::value_ptr(phy->force)),
    //                       3);
  }

  ImGui::End();

  ImGui::Render();
}

void GalaxianEditor::close() {
  //
}

/*#include "GalaxianEditor.hpp"
#include "Framework/Config.hpp"
#include "Game/Factory.hpp"
#include "Systems.hpp"
#include "imgui_impl.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <imgui.h>

void GalaxianEditor::receive(const GameResetEvent &e) {
  ClearCameras();
  m_slowmo = false;
}

bool GalaxianEditor::onCreate(int argc, char **argv) {
  // std::cout << PROJECT_DIR << "\n";

  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(GetWindowHandle().glfwWindowHandle, true);

  // Load resources
  load();

  // Add systems
  addSystems();

  // Reset level
  events.emit<GameResetEvent>();
  return true;
}

void GalaxianEditor::onUpdate(float dt) {
  // Quit app when escape is pressed
  if (IsKeyDown(KEY_ESCAPE))
    destroy();

  m_time += dt;
  if (!m_controlTime) {
    if (m_paused)
      m_timeScale = Lerp(m_timeScale, 0.f, 0.1f);
    else if (m_slowmo)
      m_timeScale = Lerp(m_timeScale, SLOWMO_TD, 0.05f);
    else
      m_timeScale = Lerp(m_timeScale, 1.f, 0.1f);
  }

  SetTimeScale(m_timeScale);
  SetTimePostFxVar(m_time);
  SetPostProcessEnabled(m_postproc);
  SetBloomKPostFxVar(m_bloomK);
  SetWaveParamsPostFxVar(m_waveParams);

  // Update cameras
  int screenWidth;
  int screenHeight;
  GetScreenSize(screenWidth, screenHeight);

  for (auto &camera : GetCameras()) {
    camera.second.orthographic(0.f, (float)screenWidth, 0.f,
                               (float)screenHeight, 0.01f, 1000.f);
  }

  // Update logic systems
  updateSystems(dt);

  // Calculate FPS
  m_fps++;
  m_fpsTimer += GetDeltaTime();
  if (m_fpsTimer >= 1.f) {
    for (auto &p : m_profiles) {
      p.frameTimeStr = std::to_string(p.frameTime * 60 * 100);
    }

    m_fpsTimer -= 1.f;
    m_fpsStr = std::to_string(m_fps);
    m_fps = 0;
  }
}

void GalaxianEditor::onDraw(float dt) {
  ClearScreen();

  // Update render systems
  renderSystems(dt);

  double mx, my;
  int sx, sy;
  GetScreenSize(sx, sy);
  GetMousePos(mx, my);
  vec4 mPos = vec4(mx, sy - my, 0, 1);
  for (auto &camera : GetCameras()) {
    mPos = glm::inverse(camera.second.getView()) * mPos;
    DrawLine(mPos.x + 2, mPos.y + 2, mPos.x - 2, mPos.y - 2, 0xFFFFFFFF,
             0xFFFFFFFF);
    DrawLine(mPos.x + 2, mPos.y - 2, mPos.x - 2, mPos.y + 2, 0xFFFFFFFF,
             0xFFFFFFFF);
  }
}

void GalaxianEditor::onEditor() {
  ImGui_ImplGlfwGL3_NewFrame();

  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("File")) {
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Edit")) {
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("View")) {
    ImGui::Checkbox("Profiler", &m_profiler);
    ImGui::Checkbox("Editor", &m_editor);
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();

  if (m_profiler) {
    ImGui::Begin("- Profiler -");
    ImGui::Text("- Stats -");
    ImGui::Text(("FPS: " + m_fpsStr).c_str());
    for (auto p : m_profiles) {
      ImGui::Text((p.name + ": " + p.frameTimeStr).c_str());
    }
    ImGui::End();
  }

  if (m_editor) {
    ImGui::Begin("- Editor -");

    ImGui::Text("- Settings -");
    ImGui::Checkbox("Debug Mode", &m_debbuging);
    ImGui::Checkbox("SloMo Mode", &m_slowmo);
    ImGui::Checkbox("Paused", &m_paused);
    ImGui::Checkbox("Control Time", &m_controlTime);
    ImGui::InputFloat("Time", &m_time, 0.1f, 1.0f, 2);
    ImGui::SliderFloat("Time Scale", &m_timeScale, -1.0f, 2.0f);

    ImGui::Checkbox(" - Post-Processing - ", &m_postproc);
    if (m_postproc) {
      ImGui::Text("- Bloom -");
      ImGui::InputFloat("BloomK", &m_bloomK, 0.5f, 2.0f, 2);
      if (m_bloomK <= 0)
        m_bloomK = 0.001f;
      ImGui::Text("- Show Wave -");
      ImGui::InputFloat("Wave Params X", &m_waveParams.x, 0.5f, 2.0f, 2);
      ImGui::InputFloat("Wave Params Y", &m_waveParams.y, 0.01f, 0.1f, 2);
      ImGui::InputFloat("Wave Params Z", &m_waveParams.z, 0.01f, 0.1f, 2);
      ImGui::InputFloat("Wave Params W", &m_waveParams.w, 0.5f, 2.0f, 2);
    }

    if (ImGui::Button("Reset", ImVec2(50, 20)))
      events.emit<GameResetEvent>();

    ImGui::End();
  }

  ImGui::Render();
}

void GalaxianEditor::onDestroy() {
  // Unload resources
  GVars::unload(*this);

  // Cleanup ImGui
  ImGui_ImplGlfwGL3_Shutdown();
}

void GalaxianEditor::load() {
  srand(time(NULL));

  // Load resources
  SetPostProcessFx((Config::ResPath + "/shaders/postpass.fsh").c_str());
  SetColorPostFxVar(vec4(1, 1, 1, 1));

  SetBackgroundColor(SColour(0x0C, 0x0C, 0x0C, 0xFF));

  GVars::load(*this);
}*/

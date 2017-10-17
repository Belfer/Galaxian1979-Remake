#include "GalaxianEditor.hpp"
#include "Core/Color.hpp"
#include "Core/Global.hpp"
#include "Core/SpriteBatch.hpp"
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

#define NUM_ENTS 500
Ent ents[NUM_ENTS];

using namespace glm;

Material material;
SpriteBatch *pSpriteBatch;

bool GalaxianEditor::init(int argc, char **args) {
  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(Engine::GetWindow().getPtr(), true);

  int w = 0, h = 0;
  Engine::GetWindow().getWindowSize(w, h);

  // Setup camera
  size_t cameraId = Engine::GetRenderer().newCamera();
  Camera &camera = Engine::GetRenderer().getCamera(cameraId);

  const float hw = w * 0.5f;
  const float hh = h * 0.5f;
  camera.orthographic(-hw, hw, -hh, hh, 1.f, 100.f);
  camera.setViewSize(vec2(w, h));

  // Setup texture
  material.texture = Engine::GetRenderer().newTexture();
  Texture &texture = Engine::GetRenderer().getTexture(material.texture);

  texture.load(Global::ResPath + "/images/invader.png", Texture::RGBA);
  texture.setMipmapFilter(Texture::NEAREST_MIPMAP_NEAREST);
  texture.setSampleFilter(Texture::NEAREST);

  // Setup shader
  material.shader = Engine::GetRenderer().newShader();
  Shader &shader = Engine::GetRenderer().getShader(material.shader);

  shader.load(Global::ResPath + "/shaders/sprite.vert",
              Global::ResPath + "/shaders/sprite.frag");

  // Setup sprite batch
  pSpriteBatch = new SpriteBatch(Engine::GetRenderer(), NUM_ENTS);
  pSpriteBatch->configure();

  for (auto &ent : ents) {
    ent.col = vec4((rand() % 100) / 100.f, (rand() % 100) / 100.f,
                   (rand() % 100) / 100.f, (rand() % 100) / 100.f);
    ent.pos = vec2((rand() % 768) - 384, (rand() % 768) - 384);
    ent.vel = vec2(cos(rand() % 360) * 1000, sin(rand() % 360) * 1000);
    ent.trq = (float)(rand() % 100);
    ent.scl = 1 + (rand() % 5) / 5.f;
  }

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

void GalaxianEditor::update(float dt) {
  // std::cout << dt << "u\n\n";
  t += dt;

  float p = glfwGetTime();
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
  std::cout << (100 * (glfwGetTime() - p) / (1 / 60.f)) << "\n";

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

void GalaxianEditor::draw(Camera &camera, float dt) {
  pSpriteBatch->draw(camera, material);

  // Update and profile render systems
  //  updateProfileSys<MenuSystem>(9, dt);
  //  updateProfileSys<ParticleSystem>(10, dt);
  //  updateProfileSys<SpriteSystem>(11, dt);
  //  updateProfileSys<TextSystem>(12, dt);
  //  updateProfileSys<LineSystem>(13, dt);
  //  if (m_debbuging)
  //    updateProfileSys<DebugSystem>(14, dt);
}

void GalaxianEditor::editor() {
  ImGui_ImplGlfwGL3_NewFrame();

  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("File")) {
    ImGui::Text("New Scene");
    ImGui::Text("Load Scene");
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Edit")) {
    ImGui::Text("Undo");
    ImGui::Text("Redo");
    ImGui::Text("Cut");
    ImGui::Text("Copy");
    ImGui::Text("Paste");
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

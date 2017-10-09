#include "GalaxianEditor.hpp"

bool GalaxianEditor::create(int argc, char **args) {
  //
}

void GalaxianEditor::update(float dt) {
  //
}

void GalaxianEditor::draw(float dt) {
  //
}

void GalaxianEditor::editor() {
  //
}

void GalaxianEditor::destroy() {
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
}

void GalaxianEditor::addSystems() {
  // Add systems
  const size_t numSys = 15;
  m_profiles.reserve(numSys);

  regProfileSys<HealthSystem>("HealthSystem");
  regProfileSys<CameraSystem>("CameraSystem");
  regProfileSys<PhysicsSystem>("PhysicsSystem");
  regProfileSys<CollisionSystem>("CollisionSystem");
  regProfileSys<PlayerSystem>("PlayerSystem");
  regProfileSys<EnemySystem>("EnemySystem");
  regProfileSys<BulletSystem>("BulletSystem");
  regProfileSys<PowerUpSystem>("PowerUpSystem");
  regProfileSys<SpawnSystem>("SpawnSystem");

  regProfileSys<MenuSystem>("MenuSystem");
  regProfileSys<ParticleSystem>("ParticleSystem");
  regProfileSys<SpriteSystem>("SpriteSystem");
  regProfileSys<TextSystem>("TextSystem");
  regProfileSys<LineSystem>("LineSystem");
  regProfileSys<DebugSystem>("DebugSystem");
  systems.configure();
}

void GalaxianEditor::updateSystems(float dt) {
  // Update and profile logic systems
  updateProfileSys<HealthSystem>(0, dt);
  updateProfileSys<CameraSystem>(1, dt);
  updateProfileSys<PhysicsSystem>(2, dt);
  updateProfileSys<CollisionSystem>(3, dt);
  updateProfileSys<PlayerSystem>(4, dt);
  updateProfileSys<EnemySystem>(5, dt);
  updateProfileSys<BulletSystem>(6, dt);
  updateProfileSys<PowerUpSystem>(7, dt);
  updateProfileSys<SpawnSystem>(8, dt);
}

void GalaxianEditor::renderSystems(float dt) {
  // Update and profile render systems
  updateProfileSys<MenuSystem>(9, dt);
  updateProfileSys<ParticleSystem>(10, dt);
  updateProfileSys<SpriteSystem>(11, dt);
  updateProfileSys<TextSystem>(12, dt);
  updateProfileSys<LineSystem>(13, dt);
  if (m_debbuging)
    updateProfileSys<DebugSystem>(14, dt);
}*/

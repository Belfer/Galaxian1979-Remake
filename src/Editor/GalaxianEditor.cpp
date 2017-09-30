#include "GalaxianEditor.hpp"
#include "Framework/Config.h"
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
  srand(time(NULL));

  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(GetWindowHandle().glfwWindowHandle, true);

  // Load resources
  SetPostProcessFx((Config::g_ResourcesPath + "/shaders/postpass.fsh").c_str());
  SetColorPostFxVar(vec4(1, 1, 1, 1));

  setBackgroundColor(SColour(0x0C, 0x0C, 0x0C, 0xFF));

  GVars::load(*this);

  // Configure event receiversr
  configure(events);

  // Add systems
  addSystems();
  systems.add<DebugSystem>(*this, entities, events, systems);
  systems.configure();

  // Reset level
  events.emit<GameResetEvent>();
  return true;
}

void GalaxianEditor::onUpdate(float deltaTime) {
  // Quit app when escape is pressed
  if (IsKeyDown(KEY_ESCAPE))
    destroy();

  if (m_paused)
    m_timeDilation = Lerp(m_timeDilation, 0.f, 0.1f);
  else if (m_slowmo)
    m_timeDilation = Lerp(m_timeDilation, SLOWMO_TD, 0.05f);
  else
    m_timeDilation = Lerp(m_timeDilation, 1.f, 0.1f);

  SetPostProcessEnabled(m_postproc);

//  if (GetMouseButtonDown(0)) {
//    double mx, my;
//    GetMousePos(mx, my);
//    events.emit<ExplosionEvent>(vec3(mx, my, 0.f));
//  }
  SetTimePostFxVar(m_time);

  // Update logic systems
  const float dt = deltaTime * m_timeDilation;
  m_time += dt;
  updateSystems(dt);
}

void GalaxianEditor::onDraw() {
  clearScreen();

  // Update render systems
  const float dt = GetDeltaTime() * m_timeDilation;
  renderSystems(dt);

#if (ENABLE_DEBUG)
  if (m_debbuging)
    systems.update<DebugSystem>(dt);
#endif
}

void GalaxianEditor::onEditor() {
  ImGui_ImplGlfwGL3_NewFrame();

  ImGui::Text("- Editor Settings -");
  ImGui::Checkbox("Debug mode", &m_debbuging);
  ImGui::Checkbox("SloMo mode", &m_slowmo);
  ImGui::Checkbox("Paused", &m_paused);
  ImGui::Checkbox("Post-Processing", &m_postproc);
  ImGui::SliderFloat("Time dilation", &m_timeDilation, 0.0f, 1.0f);
  if (ImGui::Button("Reset", ImVec2(50, 20)))
    events.emit<GameResetEvent>();

  ImGui::Render();
}

void GalaxianEditor::onDestroy() {
  // Unload resources
  GVars::unload(*this);

  // Cleanup ImGui
  ImGui_ImplGlfwGL3_Shutdown();
}

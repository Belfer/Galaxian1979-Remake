#pragma once

#include "Core/Engine.hpp"
#include "Core/Mesh.hpp"
#include "Core/Shader.hpp"
#include "Core/Texture.hpp"

using namespace NHTV;

class GalaxianEditor : public Application {
protected:
  virtual bool init(int argc, char **args) override;
  virtual void update(float dt) override;
  virtual void draw(float dt) override;
  virtual void editor() override;
  virtual void close() override;

  Texture texture;
  Shader shader;
  Mesh mesh;
};

/*#include "Game/Events.hpp"
#include "Game/GalaxianApp.hpp"
#include <entityx/entityx.h>

using namespace NHTV;
using namespace entityx;

#define SLOWMO_TD 0.25f

class GalaxianEditor : public GalaxianApp {
protected:
  virtual bool onCreate(int argc, char **argv) override;
  virtual void onUpdate(float dt) override;
  virtual void onDraw(float dt) override;
  virtual void onEditor() override;
  virtual void onDestroy() override;

  virtual void load() override;
  virtual void addSystems() override;
  virtual void updateSystems(float dt) override;
  virtual void renderSystems(float dt) override;

  template <typename T> inline void regProfileSys(const std::string &name) {
    systems.add<T>(*this, entities, events, systems);
    m_profiles.emplace_back(ProfileEntry(name));
  }

  template <typename T> inline void updateProfileSys(size_t i, float dt) {
    float t = glfwGetTime();
    systems.update<T>(dt);
    m_profiles[i].frameTime = glfwGetTime() - t;
  }

  bool m_editor = true;
  bool m_profiler = true;

  bool m_debbuging = false;
  bool m_controlTime = false;

  float m_bloomK = 81.0f;

  vec4 m_waveParams = vec4(10.f, 0.6f, 0.02f, 15.f);
  float m_fpsTimer = 0;
  uint m_fps = 0;
  std::string m_fpsStr = "";

  struct ProfileEntry {
    ProfileEntry(const std::string &name) : name(name) {}
    const std::string name;
    float frameTime = 0;
    std::string frameTimeStr = "";
  };
  std::vector<ProfileEntry> m_profiles;

public:
  void receive(const GameResetEvent &e);
};*/

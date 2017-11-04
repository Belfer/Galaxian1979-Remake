#pragma once

#include "NonCopyable.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Window.hpp"

#define APPLICATION_ENTRY(appClass)                                            \
  using namespace NHTV;                                                        \
  int main(int argc, char **args) {                                            \
    return Engine::Instance().start<appClass>(argc, args);                     \
  }

namespace NHTV {

class Application : public NonCopyable {
protected:
  friend class Engine;
  virtual bool init(int argc, char **args) = 0;
  virtual void fixed(float dt) = 0;

  virtual void pre_update(float dt) = 0;
  virtual void update(float dt) = 0;
  virtual void post_update(float dt) = 0;

  virtual void pre_draw(float dt) = 0;
  virtual void draw(Camera &camera, float dt) = 0;
  virtual void post_draw(float dt) = 0;

  virtual void editor() = 0;
  virtual void close() = 0;
};

class Engine : public NonCopyable {
public:
  static Engine &Instance() {
    static Engine *pInstance = nullptr;
    if (pInstance == nullptr)
      pInstance = new Engine();
    return *pInstance;
  }
  virtual ~Engine() {}

  template <typename App> inline int start(int argc, char **args) {
    static_assert(std::is_base_of<Application, App>::value,
                  "App must derive from Application!");
    m_running = true;
    return run(argc, args, new App());
  }

  inline void stop() { m_running = false; }

  static Window &GetWindow() { return *Instance().m_pWindow; }

  static Renderer &GetRenderer() { return *Instance().m_pRenderer; }

private:
  Engine() {}

  Window *m_pWindow;
  Renderer *m_pRenderer;

  int run(int argc, char **args, Application *app);

  bool m_running = false;
};
}

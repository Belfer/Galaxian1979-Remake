#pragma once

#include "NonCopyable.hpp"

#define APPLICATION_ENTRY(appClass)                                            \
  using namespace NHTV;                                                        \
  int main(int argc, char **args) {                                            \
    return Engine::Instance().start<appClass>(argc, args);                     \
    return 0;                                                                  \
  }

namespace NHTV {

class Application : public NonCopyable {
protected:
  friend class Engine;
  virtual bool create(int argc, char **args) = 0;
  virtual void update(float dt) = 0;
  virtual void draw(float dt) = 0;
  virtual void editor() = 0;
  virtual void destroy() = 0;
};

class Engine : public NonCopyable {
public:
  static Engine &Instance() {
    static Engine *pInstance = nullptr;
    if (pInstance == nullptr)
      pInstance = new Engine();
    return *pInstance;
  }

  template <typename App> inline int start(int argc, char **args) {
    m_running = true;
    return run(argc, args, new App());
  }

  inline void stop() { m_running = false; }

private:
  int run(int argc, char **args, Application *app);

  bool m_running = false;
};
}

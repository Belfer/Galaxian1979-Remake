#include "Engine.hpp"
#include "Global.hpp"
#include "Thread.hpp"
#include "Timer.hpp"
#include "Util.hpp"
#include <assert.h>
#include <chrono>
#include <json.hpp>
#include <thread>

using namespace NHTV;
using json = nlohmann::json;

int Engine::run(int argc, char **args, Application *app) {
  if (argc < 2)
    assert(false && "Resource path not specified!");
  Global::ResPath = args[1];

  auto appData = json::parse(Util::readFile(Global::ResPath + "/appData.json"));

  WinConfig config;
  config.title = appData["title"];
  config.xpos = appData["xPos"];
  config.ypos = appData["yPos"];
  config.width = appData["width"];
  config.height = appData["height"];

  m_pWindow = new Window(config);
  m_pRenderer = new Renderer();

  // Create a timer to track the elapsed time
  HiResTimer frameTimer;
  frameTimer.reset();

  NanoTime elapsed;
  NanoTime carry;
  NanoTime diff;
  const NanoTime frameTime = NanoTime(SecTime(1.0 / 60.0));

  app->init(argc, args);

  // Start the game loop
  while (!m_pWindow->shouldClose() && m_running) {
    elapsed = frameTimer.elapsed<Nano>(); // + carry;
    frameTimer.reset();
    // carry = NanoTime(0);
    if (elapsed < frameTime) {
      diff = frameTime - elapsed;

      // frameTimer.reset();
      Thread::sleep(diff);

      //      elapsed = frameTimer.elapsed<Nano>();
      //      if (elapsed > diff) {
      //        carry = elapsed - diff;
      //        if (carry > frameTime)
      //          carry = frameTime;
      //      }
    }
    // frameTimer.reset();

    app->update(SecTime(elapsed).count());

    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // app->pre_draw();

    int w = 0, h = 0;
    GetWindow().getWindowSize(w, h);
    for (auto &e : GetRenderer().m_cameraMap) {
      e.second.update(w, h);
      const auto &vp = e.second.getViewport();
      glViewport(vp.x * w, vp.y * h, vp.z * w, vp.w * h);

      app->draw(e.second, SecTime(elapsed).count());
    }

    // app->post_draw();

    app->editor();

    m_pWindow->display();
    m_pWindow->pollEvents();
  }

  app->close();

  return 0;
}

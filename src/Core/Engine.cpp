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
  HiResTimer carryTimer;
  carryTimer.reset();

  NanoTime elapsed;
  NanoTime diff;
  NanoTime carry;
  const NanoTime frameTime = NanoTime(SecTime(1.0 / 60.0));
  const SecTime fixedTime = SecTime(1.0 / 120.0);

  std::cout << std::fixed;
  std::cout << std::setprecision(6);

  app->init(argc, args);

  // Start the game loop
  while (!m_pWindow->shouldClose() && m_running) {
    std::cout << "Frame time: " << SecTime(frameTime).count() << "\n";

    elapsed = frameTimer.elapsed<Nano>() + carry;
    std::cout << "Elapsed: " << SecTime(elapsed).count() << "\n";
    frameTimer.reset();
    carry = NanoTime(0);

    if (elapsed < frameTime) {
      diff = frameTime - elapsed;
      std::cout << "Difference: " << SecTime(diff).count() << "\n";

      carryTimer.reset();
      Thread::sleep(diff);
      carry = diff - carryTimer.elapsed<Nano>();
      carryTimer.reset();

      // frameTimer += carry;
      std::cout << "Carry: " << SecTime(carry).count() << "\n";
    }

    app->fixed(fixedTime.count());

    const float dt = SecTime(elapsed).count();
    app->pre_update(dt);
    app->update(dt);
    app->post_update(dt);

    int w = 0, h = 0;
    GetWindow().getWindowSize(w, h);
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app->pre_draw(dt);
    for (auto &e : GetRenderer().m_cameraMap) {
      e.second.update(w, h);
      app->draw(e.second, dt);
    }
    app->post_draw(dt);

    app->editor();

    m_pWindow->display();
    m_pWindow->pollEvents();
  }

  app->close();

  delete m_pWindow;
  delete m_pRenderer;

  return 0;
}

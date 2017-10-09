#include "Engine.hpp"
#include "Config.hpp"
#include "Util.hpp"
#include "Window.hpp"
#include <assert.h>
#include <json.hpp>

using namespace NHTV;
using json = nlohmann::json;

int Engine::run(int argc, char **args, Application *app) {
  if (argc < 2)
    assert(false && "Resource path not specified!");
  Config::ResPath = args[1];

  auto appData = json::parse(readFile(Config::ResPath + "/appData.json"));

  WinConfig config;
  config.title = appData["title"];
  config.xpos = appData["xPos"];
  config.ypos = appData["yPos"];
  config.width = appData["width"];
  config.height = appData["height"];

  Window window(config);

  app->create(argc, args);
  while (!window.shouldClose() && m_running) {
    window.pollEvents();

    app->update(0);
    app->draw(0);
    app->editor();

    window.display();
  }
  app->destroy();

  return 0;
}

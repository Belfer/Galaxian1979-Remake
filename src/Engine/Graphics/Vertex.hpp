#pragma once

#include <glm/glm.hpp>

namespace NHTV {
struct Vertex {
  glm::vec4 position{0};
  glm::vec4 color{0};
  glm::vec2 texcoord{0};
};
}

#pragma once

#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace NHTV {
struct IDrawable {
protected:
  friend class Renderer;
  virtual void draw(const Camera &camera, Texture &texture, Shader &shader) = 0;
};
}

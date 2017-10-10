#pragma once

#include "Types.hpp"
#include <string>

namespace NHTV {
struct ShaderParams {
  std::string vert;
  std::string frag;
};

class Shader {
public:
  void configure(const ShaderParams &params);
  void generate();

  void load(const std::string &filename);
  void unload();

  void bind();

private:
  uint m_shader;
};
}

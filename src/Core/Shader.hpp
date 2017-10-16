#pragma once

#include "Types.hpp"
#include "NonCopyable.hpp"
#include <string>

namespace NHTV {
class Shader : public NonCopyable {
public:
  struct Params {
    std::string vert;
    std::string frag;
  };

public:
  void configure(const Params &params);

  void load(const std::string &filename);

  void bind();

  void clear();

private:
  void generate();

  uint m_shader;
  Params m_params;
};
}

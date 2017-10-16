#pragma once

#include <glad/glad.h>

#include "NonCopyable.hpp"
#include "Types.hpp"
#include <SOIL.h>
#include <string>

namespace NHTV {
class Texture : public NonCopyable {
public:
  enum WrapMode {
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
  };

  enum MipmapFilter {
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
  };

  enum SampleFilter { NEAREST = GL_NEAREST, LINEAR = GL_LINEAR };

  enum PixelChannel { RGB = SOIL_LOAD_RGB, RGBA = SOIL_LOAD_RGBA };

  struct Params {
    WrapMode wrapMode = WrapMode::REPEAT;
    MipmapFilter mipmapFilter = MipmapFilter::LINEAR_MIPMAP_LINEAR;
    SampleFilter sampleFilter = SampleFilter::LINEAR;
    uint width = 1;
    uint height = 1;
    uint bpp = 32;
    uchar *data = nullptr;
  };

public:
  void configure(const Params &params);

  void load(const std::string &filename, PixelChannel pc = PixelChannel::RGB);

  void bind();

  void clear();

private:
  void generate();

  uint m_texture;
  Params m_params;
};
}

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

  void setWrapMode(const WrapMode &wrapMode);

  void setMipmapFilter(const MipmapFilter &mipmapFilter);

  void setSampleFilter(const SampleFilter &sampleFilter);

private:
  void generate();

  uint m_texture;
  Params m_params;
};

inline void Texture::setWrapMode(const WrapMode &wrapMode) {
  m_params.wrapMode = wrapMode;
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_params.wrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_params.wrapMode);
  glBindTexture(GL_TEXTURE_2D, 0);
}

inline void Texture::setMipmapFilter(const MipmapFilter &mipmapFilter) {
  m_params.mipmapFilter = mipmapFilter;
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_params.mipmapFilter);
  glBindTexture(GL_TEXTURE_2D, 0);
}

inline void Texture::setSampleFilter(const SampleFilter &sampleFilter) {
  m_params.sampleFilter = sampleFilter;
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_params.sampleFilter);
  glBindTexture(GL_TEXTURE_2D, 0);
}
}

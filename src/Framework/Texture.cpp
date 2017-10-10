#include "Texture.hpp"
#include <cstring>
#include <iostream>

using namespace NHTV;

Texture::Texture() { glGenTextures(1, &m_texture); }

Texture::~Texture() { glDeleteTextures(1, &m_texture); }

void Texture::configure(const Params &params) {
  m_params = params;
  reconfigure();
}

void Texture::reconfigure() {
  generate();

  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_params.wrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_params.wrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_params.mipmapFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_params.sampleFilter);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::generate() {
  m_texture = SOIL_create_OGL_texture(
      m_params.data, m_params.width, m_params.height, m_params.bpp,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
          SOIL_FLAG_COMPRESS_TO_DXT);
}

void Texture::load(const std::string &filename, PixelChannel pc) {
  int w, h, bpp;
  auto img = SOIL_load_image(filename.c_str(), &w, &h, &bpp, pc);
  if (img == nullptr) {
    std::cerr << "Could not load image! " << filename << std::endl;
    return;
  }

  m_params.width = w;
  m_params.height = h;
  m_params.bpp = bpp;

  const size_t allocSize = sizeof(uchar) * w * h * 4;

  if (m_params.data != nullptr)
    free(m_params.data);
  m_params.data = static_cast<uchar *>(malloc(allocSize));

  std::memcpy(m_params.data, img, allocSize);
  SOIL_free_image_data(img);

  reconfigure();
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, m_texture); }

void Texture::destroy() {
  //
}

#include "Shader.hpp"
#include "Util.hpp"
#include <glad/glad.h>

using namespace NHTV;

void Shader::configure(const Params &params) {
  m_params = params;
  generate();
}

void Shader::generate() {
  uint vert;
  auto vertSrc = m_params.vert.c_str();
  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertSrc, NULL);
  glCompileShader(vert);

  uint frag;
  auto fragSrc = m_params.frag.c_str();
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragSrc, NULL);
  glCompileShader(frag);

  m_shader = glCreateProgram();
  glAttachShader(m_shader, vert);
  glAttachShader(m_shader, frag);
  glLinkProgram(m_shader);
}

void Shader::load(const std::string &filename) {
  std::string vert, frag;
  Util::readFile(filename + ".vert", vert);
  Util::readFile(filename + ".frag", frag);

  m_params.vert = vert;
  m_params.frag = frag;
  generate();
}

void Shader::bind() { glUseProgram(m_shader); }

void Shader::clear() {
  m_params.vert.clear();
  m_params.frag.clear();
  glDeleteProgram(m_shader);
}

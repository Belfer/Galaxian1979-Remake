#include "Shader.hpp"
#include "Engine/Core/Util.hpp"
#include <glad/glad.h>

using namespace NHTV;

void createShaderSrc(GLuint &shader, GLuint type, const std::string &src);

void checkShaderError(GLuint shader, GLuint flag, bool program,
                      const std::string &msg);

void Shader::configure(const Params &params) {
  m_params = params;
  generate();
}

void Shader::generate() {
  uint vert;
  createShaderSrc(vert, GL_VERTEX_SHADER, m_params.vert);
  checkShaderError(vert, GL_COMPILE_STATUS, false,
                   "Vertex shader compilation failed!");

  uint frag;
  createShaderSrc(frag, GL_FRAGMENT_SHADER, m_params.frag);
  checkShaderError(frag, GL_COMPILE_STATUS, false,
                   "Fragment shader compilation failed!");

  m_shader = glCreateProgram();
  glAttachShader(m_shader, vert);
  glAttachShader(m_shader, frag);

  glLinkProgram(m_shader);
  checkShaderError(m_shader, GL_LINK_STATUS, true,
                   "Could not link shader program!");

  glValidateProgram(m_shader);
  checkShaderError(m_shader, GL_LINK_STATUS, true,
                   "Shader program is invalid!");

  glDeleteShader(vert);
  glDeleteShader(frag);
}

void Shader::load(const std::string &vertfile, const std::string &fragfile) {
  std::string vert, frag;
  Util::readFile(vertfile, vert);
  Util::readFile(fragfile, frag);

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

void createShaderSrc(GLuint &shader, GLuint type, const std::string &src) {
  shader = glCreateShader(type);
  if (shader == 0) {
    std::cerr << "Shader could not be created!\n";
  }

  const GLchar *srcPtr = src.c_str();
  glShaderSource(shader, 1, &srcPtr, NULL);
  glCompileShader(shader);
}

void checkShaderError(GLuint shader, GLuint flag, bool program,
                      const std::string &msg) {
  GLint success = 0;
  GLchar errBuff[1024] = {0};

  if (program) {
    glGetProgramiv(shader, flag, &success);
  } else {
    glGetShaderiv(shader, flag, &success);
  }

  if (success == GL_FALSE) {
    if (program) {
      glGetProgramInfoLog(shader, sizeof(errBuff), NULL, errBuff);
    } else {
      glGetShaderInfoLog(shader, sizeof(errBuff), NULL, errBuff);
    }

    std::cerr << msg << " - " << errBuff << std::endl;
  }
}

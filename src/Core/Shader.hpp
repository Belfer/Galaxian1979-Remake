#pragma once

#include <glad/glad.h>

#include "NonCopyable.hpp"
#include "Types.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

using namespace glm;

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

  void setBool(const std::string &name, bool val) const;

  void setInt(const std::string &name, int val) const;

  void setFloat(const std::string &name, float val) const;

  void setVec2(const std::string &name, const vec2 &val) const;

  void setVec3(const std::string &name, const vec3 &val) const;

  void setVec4(const std::string &name, const vec4 &val) const;

  void setMat3x3(const std::string &name, const mat3x3 &val) const;

  void setMat4x4(const std::string &name, const mat4x4 &val) const;

private:
  void generate();

  uint m_shader;
  Params m_params;
};

inline void Shader::setBool(const std::string &name, bool val) const {
  glUseProgram(m_shader);
  glUniform1i(glGetUniformLocation(m_shader, name.c_str()), (int)val);
  glUseProgram(0);
}

inline void Shader::setInt(const std::string &name, int val) const {
  glUseProgram(m_shader);
  glUniform1i(glGetUniformLocation(m_shader, name.c_str()), val);
  glUseProgram(0);
}

inline void Shader::setFloat(const std::string &name, float val) const {
  glUseProgram(m_shader);
  glUniform1f(glGetUniformLocation(m_shader, name.c_str()), val);
  glUseProgram(0);
}

inline void Shader::setVec2(const std::string &name, const vec2 &val) const {
  glUseProgram(m_shader);
  glUniform2fv(glGetUniformLocation(m_shader, name.c_str()), 1,
               glm::value_ptr(val));
  glUseProgram(0);
}

inline void Shader::setVec3(const std::string &name, const vec3 &val) const {
  glUseProgram(m_shader);
  glUniform3fv(glGetUniformLocation(m_shader, name.c_str()), 1,
               glm::value_ptr(val));
  glUseProgram(0);
}

inline void Shader::setVec4(const std::string &name, const vec4 &val) const {
  glUseProgram(m_shader);
  glUniform4fv(glGetUniformLocation(m_shader, name.c_str()), 1,
               glm::value_ptr(val));
  glUseProgram(0);
}

inline void Shader::setMat3x3(const std::string &name,
                              const mat3x3 &val) const {
  glUseProgram(m_shader);
  glUniformMatrix3fv(glGetUniformLocation(m_shader, name.c_str()), 1, GL_TRUE,
                     glm::value_ptr(val));
  glUseProgram(0);
}

inline void Shader::setMat4x4(const std::string &name,
                              const mat4x4 &val) const {
  glUseProgram(m_shader);
  glUniformMatrix4fv(glGetUniformLocation(m_shader, name.c_str()), 1, GL_TRUE,
                     glm::value_ptr(val));
  glUseProgram(0);
}
}

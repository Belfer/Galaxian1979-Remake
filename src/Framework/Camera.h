#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
public:
  inline void perspective(float fovy, float aspect, float znear, float zfar) {
    m_projection = glm::perspective(fovy, aspect, znear, zfar);
  }

  inline void orthographic(float left, float right, float bottom, float top,
                           float znear, float zfar) {
    m_projection = glm::ortho(left, right, bottom, top, znear, zfar);
  }

  inline void update() {
    m_view = glm::translate(-position) * glm::toMat4(glm::conjugate(rotation));
    m_viewProj = m_view * m_projection;
    m_invViewProj = glm::inverse(m_viewProj);
  }

  inline void setViewport(const glm::vec4 &viewport) { m_viewport = viewport; }
  inline void setActive(bool active) { m_active = active; }

  inline const glm::vec4 &getViewport() const { return m_viewport; }
  inline bool isActive() const { return m_active; }

  inline const glm::mat4x4 &getView() const { return m_view; }
  inline const glm::mat4x4 &getProjection() const { return m_projection; }
  inline const glm::mat4x4 &getViewProj() const { return m_viewProj; }
  inline const glm::mat4x4 &getInvViewProj() const { return m_invViewProj; }

  glm::vec3 position = glm::vec3(0.f, 0.f, 1.f);
  glm::quat rotation = glm::quat();

private:
  bool m_active = false;

  glm::vec4 m_viewport = glm::vec4(0.f, 0.f, 1.f, 1.f);

  glm::mat4x4 m_view = glm::mat4x4(1);
  glm::mat4x4 m_projection = glm::mat4x4(1);
  glm::mat4x4 m_viewProj = glm::mat4x4(1);
  glm::mat4x4 m_invViewProj = glm::mat4x4(1);
};

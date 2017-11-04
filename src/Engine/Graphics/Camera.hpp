#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

namespace NHTV {
class Camera {
public:
  void update(int width, int height);

  bool isActive() const;
  void setActive(bool active);

  void setViewSize(int width, int height);
  const vec4 &getViewport() const;

  const mat4x4 &getView() const;
  const mat4x4 &getProjection() const;
  const mat4x4 &getViewProj() const;

  vec3 position = vec3(0.f, 0.f, 1.f);
  quat rotation = quat();

private:
  bool m_active = false;

  float m_aspect = 1;
  vec2 m_viewSize = vec2(1);
  vec4 m_viewport = vec4(0.f, 0.f, 1.f, 1.f);

  mat4x4 m_view = mat4x4(1);
  mat4x4 m_proj = mat4x4(1);
  mat4x4 m_viewProj = mat4x4(1);
};

inline bool Camera::isActive() const { return m_active; }
inline void Camera::setActive(bool active) { m_active = active; }

inline void Camera::setViewSize(int width, int height) {
  m_viewSize = vec2(width, height);
}

inline const vec4 &Camera::getViewport() const { return m_viewport; }

inline const mat4x4 &Camera::getView() const { return m_view; }
inline const mat4x4 &Camera::getProjection() const { return m_proj; }
inline const mat4x4 &Camera::getViewProj() const { return m_viewProj; }
}

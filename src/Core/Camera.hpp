#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

namespace NHTV {
class Camera {
public:
  // void orthographic(float left, float right, float bottom, float top,
  //                  float znear, float zfar);

  void update(int width, int height);
  // void bind();

  bool isActive() const;
  void setActive(bool active);

  void setViewSize(int width, int height);
  const vec4 &getViewport() const;

  const mat4x4 &getView() const;
  const mat4x4 &getProjection() const;
  const mat4x4 &getViewProj() const;

  //  inline void zoom(float amount) {
  //      m_viewSize *= amount;
  //      orthographic(m_viewSize.x);
  //  }

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

// inline void Camera::bind() {
//  glViewport(m_viewport.x * m_viewSize.x, m_viewport.y * m_viewSize.y,
//             m_viewport.z * m_viewSize.x, m_viewport.w * m_viewSize.y);
//}

inline bool Camera::isActive() const { return m_active; }
inline void Camera::setActive(bool active) { m_active = active; }

inline void Camera::setViewSize(int width, int height) {
  const float hw = width * 0.5f;
  const float hh = height * 0.5f;
  m_viewSize = vec2(width, height);
  m_proj = glm::ortho(-hw, hw, -hh, hh, 1.f, 100.f);
  // m_aspect = abs((right - left) / (top - bottom));
}

inline const vec4 &Camera::getViewport() const { return m_viewport; }

inline const mat4x4 &Camera::getView() const { return m_view; }
inline const mat4x4 &Camera::getProjection() const { return m_proj; }
inline const mat4x4 &Camera::getViewProj() const { return m_viewProj; }
}

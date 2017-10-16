#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

namespace NHTV {
class Camera {
public:
  void perspective(float fovy, float aspect, float znear, float zfar);

  void orthographic(float left, float right, float bottom, float top,
                    float znear, float zfar);

  void update();

  inline bool isActive() const { return m_active; }
  inline void setActive(bool active) { m_active = active; }

  inline void setViewSize(const vec2 &viewSize) { m_viewSize = viewSize; }
  inline const vec4 &getViewport() const { return m_viewport; }

  inline const mat4x4 &getView() const { return m_view; }
  inline const mat4x4 &getProjection() const { return m_proj; }
  inline const mat4x4 &getViewProj() const { return m_viewProj; }

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
}

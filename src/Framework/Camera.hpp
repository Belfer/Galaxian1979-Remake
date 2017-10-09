#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

class Camera {
public:
  inline void perspective(float fovy, float aspect, float znear, float zfar) {
    m_proj = glm::perspective(fovy, aspect, znear, zfar);
    m_aspect = aspect;
  }

  inline void orthographic(float left, float right, float bottom, float top,
                           float znear, float zfar) {
    m_proj = glm::ortho(left, right, bottom, top, znear, zfar);
    m_aspect = abs((right - left) / (top - bottom));
  }

  inline void update() {
    int swidth = 1024;
    int sheight = 728;

    float aspect = m_viewSize.x / m_viewSize.y;

    int vw = swidth;
    int vh = (int)(vw / aspect + .5f);

    if (vh > sheight) {
      vh = sheight;
      vw = (int)(vh * aspect + .5f);
    }

    int vx = (swidth * .5f) - (vw * .5f);
    int vy = (sheight * .5f) - (vh * .5f);

    m_viewport = vec4(vx, vy, vw, vh);
    m_view = translate(-position) * toMat4(conjugate(rotation));
    m_viewProj = m_view * m_proj;
  }

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

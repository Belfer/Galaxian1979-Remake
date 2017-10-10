#include "Camera.hpp"

using namespace NHTV;

void Camera::perspective(float fovy, float aspect, float znear, float zfar) {
  m_proj = glm::perspective(fovy, aspect, znear, zfar);
  m_aspect = aspect;
}

void Camera::orthographic(float left, float right, float bottom, float top,
                          float znear, float zfar) {
  m_proj = glm::ortho(left, right, bottom, top, znear, zfar);
  m_aspect = abs((right - left) / (top - bottom));
}

void Camera::update() {
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

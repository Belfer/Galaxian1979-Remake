#include "Camera.hpp"

using namespace NHTV;

//void Camera::orthographic(float left, float right, float bottom, float top,
//                          float znear, float zfar) {
//  m_proj = glm::ortho(left, right, bottom, top, znear, zfar);
//  m_aspect = abs((right - left) / (top - bottom));
//}

void Camera::update(int width, int height) {
  float aspect = m_viewSize.x / m_viewSize.y;

  int vw = width;
  int vh = (int)(vw / aspect + .5f);

  if (vh > height) {
    vh = height;
    vw = (int)(vh * aspect + .5f);
  }

  int vx = (width * .5f) - (vw * .5f);
  int vy = (height * .5f) - (vh * .5f);

  glViewport(vx, vy, vw, vh);

  m_viewport = vec4((float)vx / width, (float)vy / height, (float)vw / width,
                    (float)vh / height);
  m_view = translate(-position) * toMat4(conjugate(rotation));
  m_viewProj = m_view * m_proj;
}

#include "Camera.hpp"

using namespace NHTV;

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

  const float hw = m_viewSize.x * 0.5f;
  const float hh = m_viewSize.y * 0.5f;
  m_proj = ortho(-hw, hw, -hh, hh, 0.1f, 100.f);
  m_view = translate(-position) * toMat4(conjugate(rotation));
  m_viewProj = m_view * m_proj;
}

#pragma once

#include <glad/glad.h>

#include "Input.hpp"
#include "NonCopyable.hpp"
#include "Types.hpp"
#include <GLFW/glfw3.h>
#include <string>

namespace NHTV {
struct WinConfig {
  std::string title;
  uint xpos = 0, ypos = 0;
  uint width = 1, height = 1;
  bool vsync = false;
};

struct Window : NonCopyable {
  Window(const WinConfig &config);
  ~Window();

  bool shouldClose();

  void pollEvents();

  void display();

  void getWindowSize(int &width, int &height) const;

  void getFramebufferSize(int &width, int &height) const;

  void getWindowFrameSize(int &left, int &top, int &right, int &bottom) const;

  void getWindowPos(int &xpos, int &ypos) const;

  void setWindowPos(int xpos, int ypos);

  void setWindowTitle(const char *title);

  void showWindow();

  void hideWindow();

  // inline void focusWindow() { glfwFocusWindow(m_pWindow); }
  // int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
  // int visible = glfwGetWindowAttrib(window, GLFW_VISIBLE);

  int getKey(int key) const;

  int getMouseBtn(int btn) const;

  void getMousePos(double &xpos, double &ypos) const;

  int isJoystickPresent(int joy) const;

  const float *getJoystickAxes(int joy, int &count) const;

  const uchar *getJoystickBtns(int joy, int &count) const;

  const char *getJoystickName(int joy) const;

  GLFWwindow *getPtr() const;

private:
  GLFWwindow *m_pWindow;
};

inline bool Window::shouldClose() { return glfwWindowShouldClose(m_pWindow); }

inline void Window::pollEvents() { glfwPollEvents(); }

inline void Window::display() { glfwSwapBuffers(m_pWindow); }

inline void Window::getWindowSize(int &width, int &height) const {
  glfwGetWindowSize(m_pWindow, &width, &height);
}

inline void Window::getFramebufferSize(int &width, int &height) const {
  glfwGetFramebufferSize(m_pWindow, &width, &height);
}

inline void Window::getWindowFrameSize(int &left, int &top, int &right,
                                       int &bottom) const {
  glfwGetWindowFrameSize(m_pWindow, &left, &top, &right, &bottom);
}

inline void Window::getWindowPos(int &xpos, int &ypos) const {
  glfwGetWindowPos(m_pWindow, &xpos, &ypos);
}

inline void Window::setWindowPos(int xpos, int ypos) {
  glfwSetWindowPos(m_pWindow, xpos, ypos);
}

inline void Window::setWindowTitle(const char *title) {
  glfwSetWindowTitle(m_pWindow, title);
}

inline void Window::showWindow() { glfwShowWindow(m_pWindow); }

inline void Window::hideWindow() { glfwHideWindow(m_pWindow); }

// inline void focusWindow() { glfwFocusWindow(m_pWindow); }
// int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
// int visible = glfwGetWindowAttrib(window, GLFW_VISIBLE);

inline int Window::getKey(int key) const { return glfwGetKey(m_pWindow, key); }

inline int Window::getMouseBtn(int btn) const {
  return glfwGetMouseButton(m_pWindow, btn);
}

inline void Window::getMousePos(double &xpos, double &ypos) const {
  glfwGetCursorPos(m_pWindow, &xpos, &ypos);
}

inline int Window::isJoystickPresent(int joy) const {
  return glfwJoystickPresent(joy);
}

inline const float *Window::getJoystickAxes(int joy, int &count) const {
  return glfwGetJoystickAxes(joy, &count);
}

inline const uchar *Window::getJoystickBtns(int joy, int &count) const {
  return glfwGetJoystickButtons(joy, &count);
}

inline const char *Window::getJoystickName(int joy) const {
  return glfwGetJoystickName(joy);
}

inline GLFWwindow *Window::getPtr() const { return m_pWindow; }
}

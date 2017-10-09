#pragma once

#include <glad/glad.h>

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

  inline bool shouldClose() { return glfwWindowShouldClose(m_pWindow); }

  inline void pollEvents() { glfwPollEvents(); }

  inline void display() { glfwSwapBuffers(m_pWindow); }

  inline void getWindowSize(int *width, int *height) const {
    glfwGetWindowSize(m_pWindow, width, height);
  }

  inline void getFramebufferSize(int *width, int *height) const {
    glfwGetFramebufferSize(m_pWindow, width, height);
  }

  inline void getWindowFrameSize(int *left, int *top, int *right,
                                 int *bottom) const {
    glfwGetWindowFrameSize(m_pWindow, left, top, right, bottom);
  }

  inline void getWindowPos(int *xpos, int *ypos) const {
    glfwGetWindowPos(m_pWindow, xpos, ypos);
  }

  inline void setWindowPos(int xpos, int ypos) {
    glfwSetWindowPos(m_pWindow, xpos, ypos);
  }

  inline void setWindowTitle(const char *title) {
    glfwSetWindowTitle(m_pWindow, title);
  }

  inline void showWindow() { glfwShowWindow(m_pWindow); }

  inline void hideWindow() { glfwHideWindow(m_pWindow); }

  // inline void focusWindow() { glfwFocusWindow(m_pWindow); }
  // int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
  // int visible = glfwGetWindowAttrib(window, GLFW_VISIBLE);

  inline int getKey(int key) const { return glfwGetKey(m_pWindow, key); }

  inline int getMouseBtn(int btn) const {
    return glfwGetMouseButton(m_pWindow, btn);
  }

  inline void getMousePos(double *xpos, double *ypos) const {
    glfwGetCursorPos(m_pWindow, xpos, ypos);
  }

  inline int isJoystickPresent(int joy) const {
    return glfwJoystickPresent(joy);
  }

  inline const float *getJoystickAxes(int joy, int *count) const {
    return glfwGetJoystickAxes(joy, count);
  }

  inline const uchar *getJoystickBtns(int joy, int *count) const {
    return glfwGetJoystickButtons(joy, count);
  }

  inline const char *getJoystickName(int joy) const {
    return glfwGetJoystickName(joy);
  }

private:
  GLFWwindow *m_pWindow;
};

struct WindowCloseEvent {
  WindowCloseEvent(uchar window) : window(window) {}

  uchar window;
};

struct WindowSizeEvent {
  WindowSizeEvent(uchar window, int width, int height)
      : window(window), width(width), height(height) {}

  uchar window;
  int width;
  int height;
};

struct WindowPosEvent {
  WindowPosEvent(uchar window, int xpos, int ypos)
      : window(window), xpos(xpos), ypos(ypos) {}

  uchar window;
  int xpos;
  int ypos;
};

struct FramebufferSizeEvent {
  FramebufferSizeEvent(uchar window, int width, int height)
      : window(window), width(width), height(height) {}

  uchar window;
  int width;
  int height;
};

struct WindowFocusEvent {
  WindowFocusEvent(uchar window, int focused)
      : window(window), focused(focused) {}

  uchar window;
  int focused;
};

struct WindowRefreshEvent {
  WindowRefreshEvent(uchar window) : window(window) {}

  uchar window;
};

struct KeyboardEvent {
  KeyboardEvent(uchar window, int key, int scancode, int action, int mods)
      : window(window), key(key), scancode(scancode), action(action),
        mods(mods) {}

  uchar window;
  int key;
  int scancode;
  int action;
  int mods;
};

struct CharEvent {
  CharEvent(uchar window, int codepoint)
      : window(window), codepoint(codepoint) {}

  uchar window;
  uint codepoint;
};

struct MouseBtnEvent {
  MouseBtnEvent(uchar window, int button, int action, int mods)
      : window(window), button(button), action(action), mods(mods) {}

  uchar window;
  int button;
  int action;
  int mods;
};

struct MousePosEvent {
  MousePosEvent(uchar window, double xpos, double ypos)
      : window(window), xpos(xpos), ypos(ypos) {}

  uchar window;
  double xpos;
  double ypos;
};

struct MouseEnterEvent {
  MouseEnterEvent(uchar window, int entered)
      : window(window), entered(entered) {}

  uchar window;
  int entered;
};

struct ScrollEvent {
  ScrollEvent(uchar window, double xoffset, double yoffset)
      : window(window), xoffset(xoffset), yoffset(yoffset) {}

  uchar window;
  double xoffset;
  double yoffset;
};

struct JoystickEvent {
  JoystickEvent(uchar joy, int event) : joy(joy), event(event) {}

  uchar joy;
  int event;
};
}

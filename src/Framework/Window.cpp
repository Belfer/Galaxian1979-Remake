#include "Window.hpp"
#include <assert.h>
#include <iostream>

using namespace NHTV;

void errorCallback(int error, const char *description) {
  std::cerr << "Window error " << error << ": " << description << "\n";
}

void windowCloseCallback(GLFWwindow *window) {
  glfwSetWindowShouldClose(window, true);
  // Global::eventBus.broadcast<WindowCloseEvent>(0);
}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
  // Global::eventBus.broadcast<WindowSizeEvent>(0, width, height);
}

void windowPosCallback(GLFWwindow *window, int xpos, int ypos) {
  // Global::eventBus.broadcast<WindowPosEvent>(0, xpos, ypos);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  // Global::eventBus.broadcast<FramebufferSizeEvent>(0, width, height);
}

void windowFocusCallback(GLFWwindow *window, int focused) {
  // Global::eventBus.broadcast<WindowFocusEvent>(0, focused);
}

void windowRefreshCallback(GLFWwindow *window) {
  glfwSwapBuffers(window);
  // Global::eventBus.broadcast<WindowRefreshEvent>(0);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // Global::eventBus.broadcast<KeyboardEvent>(0, key, scancode, action, mods);
}

void mouseBtnCallback(GLFWwindow *window, int button, int action, int mods) {
  // Global::eventBus.broadcast<MouseBtnEvent>(0, button, action, mods);
}

void mousePosCallback(GLFWwindow *window, double xpos, double ypos) {
  // Global::eventBus.broadcast<MousePosEvent>(0, xpos, ypos);
}

void mouseEnterCallback(GLFWwindow *window, int entered) {
  // Global::eventBus.broadcast<MouseEnterEvent>(0, entered);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  // Global::eventBus.broadcast<ScrollEvent>(0, xoffset, yoffset);
}

void charCallback(GLFWwindow *window, uint codepoint) {
  // Global::eventBus.broadcast<CharEvent>(0, codepoint);
}

void joystickCallback(int joy, int event) {
  // Global::eventBus.broadcast<JoystickEvent>(joy, event);
}

Window::Window(const WinConfig &config) {
  glfwSetErrorCallback(errorCallback);
  assert(glfwInit() && "Could not initialize GLFW!");

  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  m_pWindow = glfwCreateWindow(config.width, config.height,
                               config.title.c_str(), NULL, NULL);
  if (m_pWindow == nullptr) {
    glfwTerminate();
    assert(false && "Could not create window!");
  }

  int glMajor = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MAJOR);
  int glMinor = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MINOR);
  int glRevision = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_REVISION);
  std::cout << "OpenGL Version " << glMajor << "." << glMinor << "."
            << glRevision << std::endl;

  glfwSetWindowCloseCallback(m_pWindow, windowCloseCallback);
  glfwSetWindowSizeCallback(m_pWindow, windowSizeCallback);
  glfwSetWindowPosCallback(m_pWindow, windowPosCallback);
  glfwSetFramebufferSizeCallback(m_pWindow, framebufferSizeCallback);

  glfwSetWindowFocusCallback(m_pWindow, windowFocusCallback);
  glfwSetWindowRefreshCallback(m_pWindow, windowRefreshCallback);

  glfwSetKeyCallback(m_pWindow, keyCallback);
  glfwSetCharCallback(m_pWindow, charCallback);

  glfwSetCursorPosCallback(m_pWindow, mousePosCallback);
  glfwSetMouseButtonCallback(m_pWindow, mouseBtnCallback);
  glfwSetCursorEnterCallback(m_pWindow, mouseEnterCallback);

  glfwSetScrollCallback(m_pWindow, scrollCallback);

  // glfwSetJoystickCallback(joystickCallback);

  glfwMakeContextCurrent(m_pWindow);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(config.vsync);
}

Window::~Window() {
  glfwDestroyWindow(m_pWindow);
  glfwTerminate();
}

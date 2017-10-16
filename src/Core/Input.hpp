#pragma once

#include "Types.hpp"

namespace NHTV {
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

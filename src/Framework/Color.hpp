#pragma once

#include "Types.h"

namespace NHTV {
struct Color {
  Color() { color.hex = 0xFFFFFFFF; }
  Color(uint hex) { color.hex = hex; }
  Color(uchar r, uchar g, uchar b, uchar a) {
    color.argb.r = r;
    color.argb.g = g;
    color.argb.b = b;
    color.argb.a = a;
  }
  union {
    uint hex;
    struct Colors {
      uchar a, r, g, b;
    } argb;
  } color;

  static const Color White = Color(0xFFFFFFFF);
  static const Color Black = Color(0x00000000);
  static const Color Red = Color(0xFF0000FF);
  static const Color Green = Color(0x00FF00FF);
  static const Color Blue = Color(0x0000FFFF);
};
}

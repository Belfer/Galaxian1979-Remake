#pragma once

#include "Types.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace NHTV {
struct Color;
struct HSL;

struct Color {
  union {
    uint hex;
    struct Colors {
      uchar r, g, b, a;
    } rgba;
  } color;

  Color(uint hex = 0xFFFFFFFF);
  Color(uchar r, uchar g, uchar b, uchar a = 255);

  inline uchar r() const { return color.rgba.r; }
  inline uchar g() const { return color.rgba.g; }
  inline uchar b() const { return color.rgba.b; }
  inline uchar a() const { return color.rgba.a; }
  inline uint hex() const { return color.hex; }

  static const Color White;
  static const Color Black;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Yellow;
  static const Color Cyan;

  HSL toHSL(const Color &c);
};

struct HSL {
  double hue;
  double saturation;
  double luminance;

  HSL() : hue(0), saturation(0), luminance(0) {}
  HSL(int h, int s, int l);

  Color toColor();
};

bool operator==(const Color &left, const Color &right);
bool operator!=(const Color &left, const Color &right);
Color operator+(const Color &left, const Color &right);
Color operator*(const Color &left, const Color &right);
Color &operator+=(Color &left, const Color &right);
Color &operator*=(Color &left, const Color &right);
}

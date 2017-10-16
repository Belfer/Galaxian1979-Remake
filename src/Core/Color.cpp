#include "Color.hpp"

using namespace NHTV;

const Color Color::White(0xFFFFFFFF);
const Color Color::Black(0x00000000);
const Color Color::Red(0xFF0000FF);
const Color Color::Green(0x00FF00FF);
const Color Color::Blue(0x0000FFFF);
const Color Color::Yellow(0x0000FFFF);
const Color Color::Cyan(0x0000FFFF);

Color::Color(uint hex) { color.hex = hex; }
Color::Color(uchar r, uchar g, uchar b, uchar a) {
  color.rgba.r = r;
  color.rgba.g = g;
  color.rgba.b = b;
  color.rgba.a = a;
}

HSL Color::toHSL(const Color &c) {
  // Trivial cases.
  if (c == Color::White) {
    return HSL(0, 0, 100);
  }

  if (c == Color::Black) {
    return HSL(0, 0, 0);
  }

  if (c == Color::Red) {
    return HSL(0, 100, 50);
  }

  if (c == Color::Yellow) {
    return HSL(60, 100, 50);
  }

  if (c == Color::Green) {
    return HSL(120, 100, 50);
  }

  if (c == Color::Cyan) {
    return HSL(180, 100, 50);
  }

  if (c == Color::Blue) {
    return HSL(240, 100, 50);
  }

  if (c == Color::Cyan) {
    return HSL(300, 100, 50);
  }

  double r, g, b;
  r = c.r() / 255.0;
  g = c.g() / 255.0;
  b = c.b() / 255.0;
  // Non trivial cases.
  double max, min, l, s;

  max = glm::max(glm::max(r, g), b);
  min = glm::min(glm::min(r, g), b);

  HSL A;
  l = ((max + min) / 2.0);

  if (max - min <= glm::epsilon<float>()) {
    A.hue = 0;
    A.saturation = 0;
  } else {
    double diff = max - min;

    if (A.luminance < 0.5)
      s = diff / (max + min);
    else
      s = diff / (2 - max - min);

    double diffR = (((max - r) * 60) + (diff / 2.0)) / diff;
    double diffG = (((max - g) * 60) + (diff / 2.0)) / diff;
    double diffB = (((max - b) * 60) + (diff / 2.0)) / diff;

    if (max - r <= glm::epsilon<float>()) {
      A.hue = diffB - diffG;
    } else if (max - g <= glm::epsilon<float>()) {
      A.hue = (1 * 360) / 3.0 + (diffR - diffB);
    } else if (max - b <= glm::epsilon<float>()) {
      A.hue = (2 * 360) / 3.0 + (diffG - diffR);
    }

    if (A.hue <= 0 || A.hue >= 360) {
      fmod(A.hue, 360);
    }

    s *= 100;
  }

  l *= 100;
  A.saturation = s;
  A.luminance = l;
  return A;
}

HSL::HSL(int h, int s, int l) {
  // Range control for Hue.
  if (h <= 360 && h >= 0) {
    hue = h;
  } else {
    if (h > 360) {
      hue = h % 360;
    } else if (h < 0 && h > -360) {
      hue = -h;
    } else if (h < 0 && h < -360) {
      hue = -(h % 360);
    }
  }

  // Range control for Saturation.
  if (s <= 100 && s >= 0) {
    saturation = s;
  } else {
    if (s > 100) {
      saturation = s % 100;
    } else if (s < 0 && s > -100) {
      saturation = -s;
    } else if (s < 0 && s < -100) {
      saturation = -(s % 100);
    }
  }

  // Range control for Luminance
  if (l <= 100 && l >= 0) {
    luminance = l;
  } else {
    if (l > 100) {
      luminance = l % 100;
    }
    if (l < 0 && l > -100) {
      luminance = -l;
    }
    if (l < 0 && l < -100) {
      luminance = -(l % 100);
    }
  }
}

double hueToRGB(double a, double b, double h) {
  if (h < 0)
    h += 1;
  if (h > 1)
    h -= 1;
  if ((6 * h) < 1) {
    return (a + (b - a) * 6 * h);
  }
  if ((2 * h) < 1) {
    return b;
  }
  if ((3 * h) < 2) {
    return (a + (b - a) * ((2.0 / 3.0) - h) * 6);
  }
  return a;
}

Color HSL::toColor() {
  // Reconvert to range [0,1]
  double h = hue / 360.0;
  double s = saturation / 100.0;
  double l = luminance / 100.0;

  float a = 0, b = 0;

  if (s <= glm::epsilon<float>()) {
    Color C(l * 255, l * 255, l * 255);
    return C;
  } else {
    if (l < 0.5) {
      b = l * (1 + s);
    } else {
      b = (l + s) - (s * l);
    }
    a = 2 * l - b;

    uchar cr = (255 * hueToRGB(a, b, (h + 1.0 / 3.0)));
    uchar cg = (255 * hueToRGB(a, b, h));
    uchar cb = (255 * hueToRGB(a, b, (h - 1.0 / 3.0)));
    return Color(cr, cg, cb, 1);
  }
}

bool operator==(const Color &left, const Color &right) {
  // return (left.r == right.r) && (left.g == right.g) && (left.b == right.b) &&
  //       (left.a == right.a);
  return false;
}

bool operator!=(const Color &left, const Color &right) {
  //return !(left == right);
}

Color operator+(const Color &left, const Color &right) {
  // return Color(Uint8(std::min(int(left.r) + right.r, 255)),
  //             Uint8(std::min(int(left.g) + right.g, 255)),
  //             Uint8(std::min(int(left.b) + right.b, 255)),
  //             Uint8(std::min(int(left.a) + right.a, 255)));
  return Color();
}

Color operator-(const Color &left, const Color &right) {
  // return Color(Uint8(std::max(int(left.r) - right.r, 0)),
  //             Uint8(std::max(int(left.g) - right.g, 0)),
  //             Uint8(std::max(int(left.b) - right.b, 0)),
  //             Uint8(std::max(int(left.a) - right.a, 0)));
  return Color();
}

Color operator*(const Color &left, const Color &right) {
  // return Color(
  //    Uint8(int(left.r) * right.r / 255), Uint8(int(left.g) * right.g / 255),
  //    Uint8(int(left.b) * right.b / 255), Uint8(int(left.a) * right.a / 255));
  return Color();
}

Color &operator+=(Color &left, const Color &right) {
  //return left = left + right;
}

Color &operator-=(Color &left, const Color &right) {
  //return left = left - right;
}

Color &operator*=(Color &left, const Color &right) {
  //return left = left * right;
}

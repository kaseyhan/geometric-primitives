#pragma once

#include "cinder/gl/gl.h"

namespace geometricprimitives {

class Pixel {
public:
  Pixel();
  Pixel(float red, float green, float blue);
  Pixel(float red, float green, float blue, float alpha);

  float GetRed() const;
  float GetGreen() const;
  float GetBlue() const;
  float GetAlpha() const;
  glm::vec2 GetLocation() const;

  void SetRed(float new_red);
  void SetGreen(float new_green);
  void SetBlue(float new_blue);
  void SetAlpha(float new_alpha);
  void SetRGBA(float r, float g, float b, float a);
  void SetLocation(glm::vec2 new_loc);

  /*
   * Adds the given RGBA values to this Pixel's, taking alpha value into account
   */
  void AddRGBA(float added_red, float added_green, float added_blue, float added_alpha);

private:
  float red_;
  float green_;
  float blue_;
  float alpha_;
};
}
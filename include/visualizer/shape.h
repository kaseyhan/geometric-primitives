#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace geometricprimitives {

class Shape {
public:
  virtual void Draw() = 0;
  virtual void Mutate(int max_width, int max_height) = 0;

  virtual glm::vec2 GetLocation() const = 0;
  virtual void SetLocation(glm::vec2& new_loc) = 0;

  virtual int GetWidth() = 0;
  virtual void SetWidth(int new_width) = 0;
  virtual int GetHeight() = 0;
  virtual void SetHeight(int new_height) = 0;
  virtual ci::ColorA GetColor() = 0;
  virtual void SetColor(ci::ColorA& new_color) = 0;

private:
  glm::vec2 location_;
  int width_;
  int height_;
  ci::ColorA color_;

};
}
#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace geometricprimitives {

class Shape {
public:
  virtual void Draw() = 0;
  virtual void Mutate() = 0;

  virtual glm::vec2 GetLocation() const = 0;
  virtual void SetLocation(glm::vec2& new_loc) = 0;


private:
  glm::vec2 location_;
  float width_;
  float height_;
  ci::Color8u color_;

};
}
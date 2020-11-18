#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace geometricprimitives {

class Shape {
public:
  virtual void Draw() = 0;
  void Mutate();

  glm::vec2 GetLocation() const;
  void SetLocation(glm::vec2& new_loc);


private:
  glm::vec2 location_;
  float width_;
  float height_;
  ci::Color8u color_;

};
}
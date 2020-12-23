#pragma once

#include <visualizer/shape.h>

namespace geometricprimitives {
namespace visualizer {

class Rectangle : public Shape {
public:
  Rectangle(glm::vec2& corner, float length, float width, ci::Color8u& color);

  void Draw() override;
  void Mutate() override;

  glm::vec2 GetLocation() const override;
  void SetLocation(glm::vec2& new_corner) override;

private:
  glm::vec2 top_left_corner_;
  float length_;    // vertical side / y
  float width_;     // horizontal side / x
  ci::Color8u color_;

};
}
}
#pragma once

#include <visualizer/shape.h>

namespace geometricprimitives {
namespace visualizer {

class Rectangle : public Shape {
public:
  Rectangle(glm::vec2& corner, int width, int height, ci::ColorA& color);

  void Draw() override;
  void Mutate(int max_dim) override;

  glm::vec2 GetLocation() const override;
  void SetLocation(glm::vec2& new_corner) override;

  int GetHeight() override;
  int GetWidth() override;
  ci::ColorA GetColor() override;
  void SetColor(ci::ColorA& new_color) override;
  void SetHeight(int new_height);
  void SetWidth(int new_width);

private:
  glm::vec2 top_left_corner_;
  int height_;    // vertical side / y
  int width_;     // horizontal side / x
  ci::ColorA color_;

};
}
}
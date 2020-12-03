#include <visualizer/shape.h>
#include <visualizer/rectangle.h>

namespace geometricprimitives {
namespace visualizer {

Rectangle::Rectangle(glm::vec2 &corner, int width, int height, ci::ColorA &color) : top_left_corner_(corner),
                      width_(width), height_(height), color_(color) { }

void Rectangle::Draw() {
  ci::gl::enableAlphaBlending();
  ci::gl::color(color_);
  ci::Rectf rectangle(top_left_corner_, top_left_corner_ + glm::vec2(width_, height_));
  ci::gl::drawSolidRect(rectangle);
}

void Rectangle::Mutate() {
}

glm::vec2 Rectangle::GetLocation() const { return top_left_corner_; }
void Rectangle::SetLocation(glm::vec2 &new_corner) { top_left_corner_ = new_corner; }
int Rectangle::GetHeight() { return height_; }
int Rectangle::GetWidth() { return width_; }
ci::ColorA Rectangle::GetColor() { return color_; }


}
}

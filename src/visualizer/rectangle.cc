#include <visualizer/shape.h>
#include <visualizer/rectangle.h>

namespace geometricprimitives {
namespace visualizer {

Rectangle::Rectangle(glm::vec2 &corner, float length, float width, ci::Color8u &color) : top_left_corner_(corner),
                      width_(width), length_(length), color_(color) { }

void Rectangle::Draw() {
  ci::gl::color(color_);
  ci::Rectf rectangle(top_left_corner_, top_left_corner_ + glm::vec2(width_, length_));
  ci::gl::drawSolidRect(rectangle);
}

void Rectangle::Mutate() {
}

glm::vec2 Rectangle::GetLocation() const { return top_left_corner_; }
void Rectangle::SetLocation(glm::vec2 &new_corner) { top_left_corner_ = new_corner; }


}
}

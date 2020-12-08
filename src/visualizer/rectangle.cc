#include <visualizer/shape.h>
#include <visualizer/rectangle.h>
#include <random>

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

void Rectangle::Mutate(int max_width, int max_height) {
  std::uniform_real_distribution<float> h(1,max_height);
  std::uniform_real_distribution<float> w(1,max_width);
  static std::default_random_engine generator;
  width_ = w(generator);
  height_ = h(generator);
}

glm::vec2 Rectangle::GetLocation() const { return top_left_corner_; }
void Rectangle::SetLocation(glm::vec2 &new_corner) { top_left_corner_ = new_corner; }

int Rectangle::GetHeight() { return height_; }
void Rectangle::SetHeight(int new_height) { height_ = new_height; }

int Rectangle::GetWidth() { return width_; }
void Rectangle::SetWidth(int new_width) { width_ = new_width; }

ci::ColorA Rectangle::GetColor() { return color_; }
void Rectangle::SetColor(ci::ColorA &new_color) { color_ = new_color; }

}
}

#include <visualizer/renderer.h>
#include <core/image.h>
#include <visualizer/shape.h>
#include <visualizer/rectangle.h>
#include <core/pixel.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <vector>
#include <random>

namespace geometricprimitives {
namespace visualizer {
using std::vector;

Renderer::Renderer() {}
Renderer::Renderer(Image& original) : original_image_(original){
  background_color_ = CalculateBackgroundColor();
  std::vector<std::vector<Pixel>> blank_pixels(original.GetPixelArray().size(),
                                               std::vector<Pixel>(original.GetPixelArray()[0].size(),
                                                                  Pixel(0,0,0,0)));
  generated_image_.SetPixelArray(blank_pixels);
}

void Renderer::draw() {
  ci::gl::color(background_color_);
  ci::Rectf background(top_left_corner_, top_left_corner_ + glm::vec2(original_image_.GetPixelArray()[0].size(),
                                                                      original_image_.GetPixelArray().size()));
  ci::gl::drawSolidRect(background);

  for (size_t i = 0; i < shapes_.size(); i++) {
    shapes_[i]->Draw();
  }
}

void Renderer::AddShape() {
  double rms = std::numeric_limits<double>::max();
  Shape* shape;
  size_t counter = 0;
  do {
    Shape* random_shape = GenerateRandomShape();
    double new_rms = CalculateRootMeanSquare(random_shape);
    if (new_rms < rms) {
      rms = new_rms;
      shape = random_shape;
    }
    counter++;
  } while (rms > kMinError && counter < kMaxRandomShapeTries);
  shapes_.push_back(shape);
}

ci::ColorA Renderer::CalculateBackgroundColor() {
  vector<vector<Pixel>> pixels = original_image_.GetPixelArray();

  float red_total = 0;
  float green_total = 0;
  float blue_total = 0;

  for (vector<Pixel>& row : pixels) {
    for (Pixel& pix : row) {
      red_total += pix.GetRed();
      green_total += pix.GetGreen();
      blue_total += pix.GetBlue();
    }
  }

  float avg_red = red_total / pixels.size() / 255;
  float avg_green = green_total / pixels.size() / 255;
  float avg_blue = blue_total / pixels.size() / 255;
  ci::ColorA background_color(avg_red, avg_green, avg_blue, 1);
  return background_color;
}

Shape* Renderer::GenerateRandomShape() const {
  int max_x = original_image_.GetPixelArray()[0].size();
  int max_y = original_image_.GetPixelArray().size();
  int max_width = kMaxDimension;
  int max_height = kMaxDimension;

  if (shapes_.size() > kNumShapes / 2) {
    max_width /= 20;
    max_height /= 20;
  } else if (shapes_.size() > kNumShapes / 10) {
    max_width /= 10;
    max_height /= 10;
  } else if (shapes_.size() > kNumShapes / 20) {
    max_width /= 2;
    max_height /= 2;
  }
  std::uniform_real_distribution<float> loc_x(0, max_x);
  std::uniform_real_distribution<float> loc_y(0, max_y);
  std::uniform_real_distribution<float> width(0, max_width);
  std::uniform_real_distribution<float> height(0, max_height);
  std::uniform_real_distribution<float> rgb_value(0,1);
  static std::default_random_engine generator;

  glm::vec2 loc((int)loc_x(generator), (int)loc_y(generator));
  ci::ColorA color(rgb_value(generator), rgb_value(generator), rgb_value(generator), kAlpha);

  return new Rectangle(loc, (int)width(generator), (int)height(generator), color);
}

double Renderer::CalculateRootMeanSquare(Shape* added_shape) {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
  double total_error = 0;
  glm::vec2 loc = added_shape->GetLocation();

  for (size_t row = 0; row < orig_pixels.size(); row++) {
    for (size_t col = 0; col < orig_pixels[0].size(); col++) {
      //rectangle
      if (row >= loc.y && row <= loc.y + added_shape->GetHeight()  &&
          col >= loc.x && col <= loc.x + added_shape->GetWidth()) {
        new_pixels[row][col].SetRGBA(added_shape->GetColor().r,
                                     added_shape->GetColor().g,
                                     added_shape->GetColor().b,1);
      }
      total_error += std::pow(std::abs(new_pixels[row][col].GetRed() - orig_pixels[row][col].GetRed()),2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetGreen() - orig_pixels[row][col].GetGreen()),2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetBlue() - orig_pixels[row][col].GetBlue()),2);
    }
  }
  generated_image_.SetPixelArray(new_pixels);
  double rms = std::pow(total_error / orig_pixels.size(), .5);
  return rms;
}

void Renderer::SetOriginalImage(Image &original) {
  original_image_ = original;
  background_color_ = CalculateBackgroundColor();
  std::vector<std::vector<Pixel>> blank_pixels(original.GetPixelArray().size(),
                                               std::vector<Pixel>(original.GetPixelArray()[0].size(),
                                                                  Pixel(0,0,0,0)));
  generated_image_.SetPixelArray(blank_pixels);
}

void Renderer::Clear() {
  for (size_t i = 0; i < shapes_.size(); i++) {
    delete shapes_[i];
  }
}
}
}
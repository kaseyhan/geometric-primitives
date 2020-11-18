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

//Renderer::Renderer() {}
Renderer::Renderer(Image& original) : original_image_(original){
  background_color_ = CalculateBackgroundColor();
}

void Renderer::AddShape() {
  double rms = std::numeric_limits<double>::max();
  Shape* shape;
  do {
    Shape* random_shape = GenerateRandomShape();
    double new_rms = CalculateRootMeanSquare();
    if (new_rms < rms) {
      rms = new_rms;
      shape = random_shape;
    }
  } while (rms > kMinError);
  shapes_.push_back(*shape);
}

ci::Color8u Renderer::CalculateBackgroundColor() {
  vector<vector<Pixel>> pixels = original_image_.GetPixelArray();

  size_t red_total = 0;
  size_t green_total = 0;
  size_t blue_total = 0;

  for (vector<Pixel>& row : pixels) {
    for (Pixel& pix : row) {
      red_total += pix.GetRed();
      green_total += pix.GetGreen();
      blue_total += pix.GetBlue();
    }
  }
  size_t num_pixels = pixels.size() * pixels[0].size();
  float avg_red = (float)red_total / num_pixels;
  float avg_green = (float)green_total / num_pixels;
  float avg_blue = (float)blue_total / num_pixels;
  ci::Color8u background_color(avg_red, avg_green, avg_blue);
  return background_color;
}

Shape* Renderer::GenerateRandomShape() const {
  std::uniform_real_distribution<float> loc_x(0,kWindowSize);
  std::uniform_real_distribution<float> loc_y(0,kWindowSize);
  std::uniform_real_distribution<float> width(0,kMaxDimension);
  std::uniform_real_distribution<float> length(0,kMaxDimension);
  std::uniform_real_distribution<float> rgb_value(0,255);
  std::default_random_engine generator;
  glm::vec2 loc(loc_x(generator), loc_y(generator));
  ci::Color8u color(rgb_value(generator), rgb_value(generator), rgb_value(generator));

  Rectangle rect(loc, length(generator), width(generator), color);
  return &rect;
}

void Renderer::draw() {
  ci::gl::color(background_color_);
  ci::Rectf background(top_left_corner_, top_left_corner_ + glm::vec2(kWindowSize, kWindowSize));
  ci::gl::drawSolidRect(background);
  for (size_t i = 0; i < shapes_.size(); i++) {
    shapes_[i].Draw();
  }
}

double Renderer::CalculateRootMeanSquare() const {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
//  double total_red_error = 0;
//  double total_green_error = 0;
//  double total_blue_error = 0;
  double total_error = 0;
  size_t total_num = orig_pixels.size() * orig_pixels[0].size();
  for (size_t row = 0; row < orig_pixels.size(); row++) {
    for (size_t col = 0; col < orig_pixels[0].size(); col++) {
      total_error += std::pow(std::abs(new_pixels[row][col].GetRed() - orig_pixels[row][col].GetRed()),2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetGreen() - orig_pixels[row][col].GetGreen()),2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetBlue() - orig_pixels[row][col].GetBlue()),2);
    }
  }
  double rms = std::pow(total_error / total_num, .5);
  return rms;
}

}
}
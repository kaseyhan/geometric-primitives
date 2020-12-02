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
  shapes_.push_back(shape);
}

ci::ColorA Renderer::CalculateBackgroundColor() {
  vector<Pixel> pixels = original_image_.GetPixelArray();

  float red_total = 0;
  float green_total = 0;
  float blue_total = 0;

  for (Pixel& pix : pixels) {
      red_total += pix.GetRed();
      green_total += pix.GetGreen();
      blue_total += pix.GetBlue();
  }

  float avg_red = red_total / pixels.size();
  float avg_green = green_total / pixels.size();
  float avg_blue = blue_total / pixels.size();
  ci::ColorA background_color(avg_red, avg_green, avg_blue, 1);
  return background_color;
}

Shape* Renderer::GenerateRandomShape() const {
  std::uniform_real_distribution<float> loc_x(0,kWindowSize);
  std::uniform_real_distribution<float> loc_y(0,kWindowSize);
  std::uniform_real_distribution<float> width(0,kMaxDimension);
  std::uniform_real_distribution<float> length(0,kMaxDimension);
  std::uniform_real_distribution<float> rgb_value(0,1);
  std::default_random_engine generator;
  glm::vec2 loc(loc_x(generator), loc_y(generator));
  ci::ColorA color(rgb_value(generator), rgb_value(generator), rgb_value(generator), kAlpha);

  //Rectangle rect(loc, length(generator), width(generator), color);
  return new Rectangle(loc, length(generator), width(generator), color);
}

void Renderer::draw() {
  ci::gl::color(background_color_);
  ci::Rectf background(top_left_corner_, top_left_corner_ + glm::vec2(kWindowSize, kWindowSize));
  ci::gl::drawSolidRect(background);
  for (size_t i = 0; i < shapes_.size(); i++) {
    shapes_[i]->Draw();
  }
}

double Renderer::CalculateRootMeanSquare() const {
  vector<Pixel> orig_pixels = original_image_.GetPixelArray();
  vector<Pixel> new_pixels = generated_image_.GetPixelArray();
//  double total_red_error = 0;
//  double total_green_error = 0;
//  double total_blue_error = 0;
  double total_error = 0;
  for (size_t i = 0; i < orig_pixels.size(); i++) {
      total_error += std::pow(std::abs(new_pixels[i].GetRed() - orig_pixels[i].GetRed()),2);
      total_error += std::pow(std::abs(new_pixels[i].GetGreen() - orig_pixels[i].GetGreen()),2);
      total_error += std::pow(std::abs(new_pixels[i].GetBlue() - orig_pixels[i].GetBlue()),2);
  }

  double rms = std::pow(total_error / orig_pixels.size(), .5);
  return rms;
}

void Renderer::SetOriginalImage(Image &original) { original_image_ = original; }

void Renderer::Clear() {
  for (size_t i = 0; i < shapes_.size(); i++) {
    delete shapes_[i];
  }
}
}
}
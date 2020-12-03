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

  std::vector<std::vector<Pixel>> blank_pixels;
  for (size_t i = 0; i < original.GetPixelArray().size(); i++) {
    std::vector<Pixel> row;
    for (size_t j = 0; j < original.GetPixelArray()[0].size(); j++) {
      Pixel pixel(0,0,0,0);
      row.push_back(pixel);
    }
    blank_pixels.push_back(row);
  }
  generated_image_.SetPixelArray(blank_pixels);
}

void Renderer::AddShape() {
  double rms = std::numeric_limits<double>::max();
  Shape* shape;
  do {
    Shape* random_shape = GenerateRandomShape();
    double new_rms = CalculateRootMeanSquare(random_shape);
    if (new_rms < rms) {
      rms = new_rms;
      shape = random_shape;
    }
  } while (rms > kMinError);
  shapes_.push_back(shape);
}

ci::ColorA Renderer::CalculateBackgroundColor() {
  vector<vector<Pixel>> pixels = original_image_.GetPixelArray();

  float red_total = 0;
  float green_total = 0;
  float blue_total = 0;

  for (vector<Pixel> row : pixels) {
    for (Pixel& pix : row) {
      red_total += pix.GetRed();
      green_total += pix.GetGreen();
      blue_total += pix.GetBlue();
    }
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
  std::uniform_real_distribution<float> height(0,kMaxDimension);
  std::uniform_real_distribution<float> rgb_value(0,1);
  std::default_random_engine generator;
  glm::vec2 loc(loc_x(generator), loc_y(generator));
  ci::ColorA color(rgb_value(generator), rgb_value(generator), rgb_value(generator), kAlpha);

  //Rectangle rect(loc, length(generator), width(generator), color);
  return new Rectangle(loc, width(generator), height(generator), color);
}

void Renderer::draw() {
  ci::gl::color(background_color_);
  ci::Rectf background(top_left_corner_, top_left_corner_ + glm::vec2(kWindowSize, kWindowSize));
  ci::gl::drawSolidRect(background);
  for (size_t i = 0; i < shapes_.size(); i++) {
    shapes_[i]->Draw();
  }
}

double Renderer::CalculateRootMeanSquare(Shape* added_shape) const {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
//  double total_red_error = 0;
//  double total_green_error = 0;
//  double total_blue_error = 0;
  double total_error = 0;
  for (size_t row = 0; row < orig_pixels.size(); row++) {
    for (size_t col = 0; col < orig_pixels[0].size(); col++) {
      //rectangle
      glm::vec2 loc = added_shape->GetLocation();
      if (row >= loc.y && row <= loc.y + added_shape->GetWidth() &&
          col >= loc.x && col <= loc.x + added_shape->GetHeight()) {
        new_pixels[row][col].SetRed(added_shape->GetColor().r);
        new_pixels[row][col].SetGreen(added_shape->GetColor().g);
        new_pixels[row][col].SetBlue(added_shape->GetColor().b);
      }

      total_error += std::pow(std::abs(new_pixels[row][col].GetRed() - orig_pixels[row][col].GetRed()),2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetGreen() - orig_pixels[row][col].GetGreen()),2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetBlue() - orig_pixels[row][col].GetBlue()),2);
    }
  }

  double rms = std::pow(total_error / orig_pixels.size(), .5);
  return rms;
}

void Renderer::SetOriginalImage(Image &original) {
  original_image_ = original;
  std::vector<std::vector<Pixel>> blank_pixels;
  for (size_t i = 0; i < original.GetPixelArray().size(); i++) {
    std::vector<Pixel> row;
    for (size_t j = 0; j < original.GetPixelArray()[0].size(); j++) {
      Pixel pixel(0,0,0,0);
      row.push_back(pixel);
    }
    blank_pixels.push_back(row);
  }
  generated_image_.SetPixelArray(blank_pixels);
}

void Renderer::Clear() {
  for (size_t i = 0; i < shapes_.size(); i++) {
    delete shapes_[i];
  }
}
}
}
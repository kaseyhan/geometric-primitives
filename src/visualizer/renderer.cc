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
  size_t rows = original_image_.GetPixelArray().size();
  size_t cols = original_image_.GetPixelArray()[0].size();
  skip_ = (int)((rows * cols) * kSkipProportion);
  if (skip_ == 0) skip_ = 1;
  max_dim_ = std::min(rows,cols);
  background_color_ = CalculateBackgroundColor();
  std::vector<std::vector<Pixel>> blank_pixels(rows,std::vector<Pixel>(cols,Pixel(background_color_.r,
                                                                                  background_color_.g, background_color_.b, 1)));
  generated_image_.SetPixelArray(blank_pixels);
}

void Renderer::draw() {
  ci::gl::clear();
  ci::gl::color(background_color_);
  ci::Rectf background(top_left_corner_, top_left_corner_ + glm::vec2(original_image_.GetPixelArray()[0].size(),
                                                                      original_image_.GetPixelArray().size()));
  ci::gl::drawSolidRect(background);

  for (size_t i = 0; i < shapes_.size(); i++) {
    shapes_[i]->Draw();
  }
}

void Renderer::AddShape() {
  Shape* shape = GenerateInitialShape();
  AdjustShapeColor(shape);
  AdjustShapeSize(shape);

  shapes_.push_back(shape);
  AddShapeToGeneratedImage(shape);
}

Shape* Renderer::GenerateInitialShape() {
  double rms = std::numeric_limits<double>::max();
  Shape* shape = nullptr;
  size_t counter = 0;
  do {
    Shape* random_shape = GenerateRandomShape();
    double new_rms = CalculatePartialRootMeanSquare(random_shape);
    if (new_rms < rms) {
      rms = new_rms;
      delete shape;
      shape = random_shape;
    }
    counter++;
  } while (rms > kMinError * 2 && counter < kMaxRandomShapeTries);

  return shape;
}

void Renderer::AdjustShapeColor(Shape *shape) {
  double rms = std::numeric_limits<double>::max();
  size_t counter = 0;

  do {
    ci::ColorA old_color = shape->GetColor();
    ci::ColorA new_color = GenerateRandomColor();
    shape->SetColor(new_color);
    double new_rms = CalculatePartialRootMeanSquare(shape);

    if (new_rms < rms) rms = new_rms;
    else shape->SetColor(old_color);

    counter++;
  } while (rms > kMinError * 1.5 && counter < kMaxColorChanges);
  std::cout<< "HI";                                                             // TEST
}

void Renderer::AdjustShapeSize(Shape *shape) {
  double rms = std::numeric_limits<double>::max();
  size_t counter = 0;

  int max_x = original_image_.GetPixelArray()[0].size();
  int max_y = original_image_.GetPixelArray().size();
  ci::Rectf canvas(top_left_corner_, top_left_corner_ + glm::vec2(max_x, max_y));
  glm::vec2 loc = shape->GetLocation();

  do {
    int old_height = shape->GetHeight();
    int old_width = shape->GetWidth();
//    shape->Mutate(max_width, max_height);
    std::vector<int> rand_dim = GenerateRandomShapeDimensions(loc);
    shape->SetWidth(rand_dim[0]);
    shape->SetHeight(rand_dim[1]);
    double new_rms = CalculatePartialRootMeanSquare(shape);

    if (new_rms < rms) {
      rms = new_rms;
    } else {
      shape->SetHeight(old_height);
      shape->SetWidth(old_width);
    }
    counter++;
  } while (rms > kMinError && counter < kMaxMutations);
  std::cout<< "HI";                                             // TEST
}

ci::ColorA Renderer::CalculateBackgroundColor() {
  vector<vector<Pixel>> pixels = original_image_.GetPixelArray();

  float red_total = 0;
  float green_total = 0;
  float blue_total = 0;

  for (size_t row = 0; row < pixels.size(); row+=skip_) {
    for (size_t col = 0; col < pixels[0].size(); col+=skip_) {
      red_total += pixels[row][col].GetRed();
      green_total += pixels[row][col].GetGreen();
      blue_total += pixels[row][col].GetBlue();
    }
  }

  int skip_factor = skip_ * skip_;

  float avg_red = red_total / (pixels.size() * pixels[0].size() / skip_factor);
  float avg_green = green_total / (pixels.size() * pixels[0].size() / skip_factor);
  float avg_blue = blue_total / (pixels.size() * pixels[0].size() / skip_factor);
  ci::ColorA background_color(avg_red, avg_green, avg_blue, 1);
  return background_color;
}

Shape* Renderer::GenerateRandomShape() {
  int max_x = original_image_.GetPixelArray()[0].size();
  int max_y = original_image_.GetPixelArray().size();
//  ci::Rectf canvas(top_left_corner_, top_left_corner_ + glm::vec2(max_x, max_y));

  ci::ColorA color = GenerateRandomColor();
  glm::vec2 loc = GenerateRandomLocation(max_x, max_y);
  std::vector<int> dimensions = GenerateRandomShapeDimensions(loc);

  return new Rectangle(loc, dimensions[0], dimensions[1], color);
}

ci::ColorA Renderer::GenerateRandomColor() {
  std::uniform_real_distribution<float> rgb_value(0,1);
  static std::default_random_engine generator;

  return ci::ColorA(rgb_value(generator), rgb_value(generator), rgb_value(generator), kAlpha);
}

glm::vec2 Renderer::GenerateRandomLocation(int max_x, int max_y) {
  std::uniform_real_distribution<float> loc_x(top_left_corner_.x - max_dim_/8, (top_left_corner_.x + max_x) * .97f);
  std::uniform_real_distribution<float> loc_y(top_left_corner_.y - max_dim_/8, (top_left_corner_.y + max_y) * .97f);
  static std::default_random_engine generator;

  return glm::vec2((int)loc_x(generator), (int)loc_y(generator));;
}

std::vector<int> Renderer::GenerateRandomShapeDimensions(glm::vec2& loc) {
  int max_width = max_dim_;
  int max_height = max_dim_;

  if (shapes_.size() > kNumShapes / 2) {
    max_width /= 10;
    max_height /= 10;
  } else if (shapes_.size() > kNumShapes / 5) {
    max_width /= 7;
    max_height /= 7;
  } else if (shapes_.size() > kNumShapes / 10) {
    max_width /= 3;
    max_height /= 3;
  } else if (shapes_.size() > kNumShapes / 20) {
    max_width /= 1.4;
    max_height /= 1.4;
  }

  int min_width = max_width * (1 - shapes_.size() / kNumShapes) * 0.6;
  int min_height = max_height * (1 - shapes_.size() / kNumShapes) * 0.6;
  if (loc.x < top_left_corner_.x) min_width = top_left_corner_.x - loc.x + 5;
  if (loc.y < top_left_corner_.y) min_height = top_left_corner_.y - loc.y + 5;
  if (loc.x > (top_left_corner_.x + max_dim_) * 0.65) max_width *= 0.3;
  if (loc.y > (top_left_corner_.y + max_dim_) * 0.65) max_height *= 0.3;

  if (min_width > max_width) {
    max_width += min_width - max_width + 5;
  }
  if (min_height > max_height) {
    max_height += min_height - max_height + 5;
  }

  if (loc.x + min_width < top_left_corner_.x) min_width += top_left_corner_.x - (loc.x + min_width) + 5;            // last check
  if (loc.y + min_height < top_left_corner_.y) min_height += top_left_corner_.y - (loc.y + min_height) + 5;

  std::uniform_real_distribution<float> width(min_width, max_width);
  std::uniform_real_distribution<float> height(min_height, max_height);
  static std::default_random_engine generator;

  return std::vector<int> {(int)width(generator), (int)height(generator)};
}

double Renderer::CalculateRootMeanSquare(Shape* added_shape) {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
  double total_error = 0;
  glm::vec2 loc = added_shape->GetLocation();
  size_t row_start = 0;
  size_t col_start = 0;
  size_t row_end = std::min(orig_pixels.size(), ((unsigned int) loc.y + added_shape->GetHeight()));
  size_t col_end = std::min(orig_pixels[0].size(), ((unsigned int) loc.x + added_shape->GetWidth()));

  if (loc.y <= top_left_corner_.y) row_start = 0;   // if the shape is further left than the image
  else row_start = loc.y - top_left_corner_.y;      // if the shape's x starts on the image

  if (loc.x <= top_left_corner_.x) col_start = 0;
  else col_start = loc.x - top_left_corner_.x;

//  if (loc.y + added_shape->GetHeight() >= orig_pixels.size()) row_end = orig_pixels.size();  // if the shape ends off of the image
//  else row_end = loc.y + added_shape->GetHeight();                                           // if the shape ends on the image

//  if (loc.x + added_shape->GetWidth() >= orig_pixels[0].size()) col_end = orig_pixels[0].size();

  for (size_t row = 0; row < orig_pixels.size(); row++) {
    for (size_t col = 0; col < orig_pixels[0].size(); col++) {
      //rectangle
      if (row >= row_start && row <= row_end && col >= col_start && col <= col_end) {
        new_pixels[row][col].AddRGBA(added_shape->GetColor().r,
                                     added_shape->GetColor().g,
                                     added_shape->GetColor().b, kAlpha);
      }
      total_error += std::pow(std::abs(new_pixels[row][col].GetRed() - orig_pixels[row][col].GetRed()), 2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetGreen() - orig_pixels[row][col].GetGreen()), 2);
      total_error += std::pow(std::abs(new_pixels[row][col].GetBlue() - orig_pixels[row][col].GetBlue()), 2);
    }
  }
  //generated_image_.SetPixelArray(new_pixels);
  double rms = std::pow(total_error / orig_pixels.size(), .5);
  return rms;
}

double Renderer::CalculatePartialRootMeanSquare(Shape* added_shape) {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
  double total_error = 0;
  glm::vec2 loc = added_shape->GetLocation();
  size_t row_start = 0; // if the shape starts further up than the image
  size_t col_start = 0;
  if (loc.y > top_left_corner_.y) row_start = loc.y - top_left_corner_.y;      // if the shape's y starts on the image
  if (loc.x > top_left_corner_.x) col_start = loc.x - top_left_corner_.x;
  size_t row_end = std::min(orig_pixels.size(), ((unsigned int) loc.y + added_shape->GetHeight()));
  size_t col_end = std::min(orig_pixels[0].size(), ((unsigned int) loc.x + added_shape->GetWidth()));

  for (size_t row = row_start; row < row_end; row++) {
    for (size_t col = col_start; col < col_end; col++) {
      new_pixels[row][col].AddRGBA(added_shape->GetColor().r,
                                     added_shape->GetColor().g,
                                     added_shape->GetColor().b,kAlpha);

      total_error += std::pow(new_pixels[row][col].GetRed() - orig_pixels[row][col].GetRed(),2);
      total_error += std::pow(new_pixels[row][col].GetGreen() - orig_pixels[row][col].GetGreen(),2);
      total_error += std::pow(new_pixels[row][col].GetBlue() - orig_pixels[row][col].GetBlue(),2);
    }
  }
  double rms = total_error / std::pow(((row_end - row_start) * (col_end - col_start)),2);
  return rms;
}

double Renderer::CalculateRootMeanSquareFaster(Shape* added_shape) {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
  double total_error = 0;
  glm::vec2 loc = added_shape->GetLocation();
  size_t row_start = 0; // if the shape starts further up than the image
  size_t col_start = 0;
  if (loc.y > top_left_corner_.y) row_start = loc.y - top_left_corner_.y;      // if the shape's y starts on the image
  if (loc.x > top_left_corner_.x) col_start = loc.x - top_left_corner_.x;

  size_t row_end = std::min(orig_pixels.size(), ((unsigned int) loc.y + added_shape->GetHeight()));
  size_t col_end = std::min(orig_pixels[0].size(), ((unsigned int) loc.x + added_shape->GetWidth()));

  for (size_t row = 0; row < orig_pixels.size(); row+=skip_) {
    for (size_t col = 0; col < orig_pixels[0].size(); col+=skip_) {
      if (row >= row_start && row <= row_end && col >= col_start && col <= col_end) {
        new_pixels[row][col].AddRGBA(added_shape->GetColor().r,
                                     added_shape->GetColor().g,
                                     added_shape->GetColor().b, kAlpha);

      }
      total_error += std::pow(new_pixels[row][col].GetRed() - orig_pixels[row][col].GetRed(), 2);
      total_error += std::pow(new_pixels[row][col].GetGreen() - orig_pixels[row][col].GetGreen(), 2);
      total_error += std::pow(new_pixels[row][col].GetBlue() - orig_pixels[row][col].GetBlue(), 2);
    }
  }
  double skip_factor = skip_ * skip_;
  double rms = std::sqrt(total_error / ((orig_pixels.size() * orig_pixels[0].size())/skip_factor));
  return rms;
}

void Renderer::AddShapeToGeneratedImage(Shape *shape) {
  glm::vec2 loc = shape->GetLocation();
  std::vector<std::vector<Pixel>> pixels = generated_image_.GetPixelArray();
  int row_start = 0; // if the shape starts further up than the image
  int col_start = 0;
  if (loc.y > top_left_corner_.y) row_start = loc.y - top_left_corner_.y;   // if the shape's y starts on the image
  if (loc.x > top_left_corner_.x) col_start = loc.x - top_left_corner_.x;

  int row_end = 0;
  int col_end = 0;
  if (loc.y + shape->GetHeight() >= top_left_corner_.y + pixels.size())
    row_end = pixels.size();  // if the shape ends off of the image
  else
    row_end = shape->GetHeight() + loc.y - top_left_corner_.y; // if the shape ends on the image

  if (loc.x + shape->GetWidth() >= top_left_corner_.x + pixels[0].size()) col_end = pixels[0].size();
  else col_end = shape->GetWidth() + loc.x - top_left_corner_.x;
  ci::ColorA color = shape->GetColor();

  for (size_t row = row_start; row < row_end; row++) {
    for (size_t col = col_start; col < col_end; col++) {
      pixels[row][col].AddRGBA(color.r, color.g, color.b, kAlpha);
    }
  }

  generated_image_.SetPixelArray(pixels);
}

void Renderer::SetOriginalImage(Image &original) {
  original_image_ = original;
  size_t rows = original_image_.GetPixelArray().size();
  size_t cols = original_image_.GetPixelArray()[0].size();
  skip_ = (int)((rows * cols) * kSkipProportion);
  if (skip_ == 0) skip_ = 1;
  max_dim_ = std::min(rows,cols);
  background_color_ = CalculateBackgroundColor();
  std::vector<std::vector<Pixel>> blank_pixels(rows,std::vector<Pixel>(cols,Pixel(background_color_.r,
                                                                                  background_color_.g, background_color_.b, 1)));
  generated_image_.SetPixelArray(blank_pixels);
}

void Renderer::Clear() {
  for (size_t i = 0; i < shapes_.size(); i++) {
    delete shapes_[i];
  }
}
}
}
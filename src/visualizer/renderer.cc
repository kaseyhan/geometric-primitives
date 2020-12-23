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
  AdjustShapeSize(shape);
  shapes_.push_back(shape);
  AddShapeToGeneratedImage(shape);
}

Shape* Renderer::GenerateInitialShape() {
  double rms = std::numeric_limits<double>::max();
  Shape* shape = nullptr;
  size_t counter = 0;
  do {
    Shape* random_shape = GenerateRandomShape();                        // 1) generate a random shape
    double new_rms = CalculateRootMeanSquare(random_shape);             // 2) calculate the error
    if (new_rms < rms) {                                                // 3) if the error has improved,
      rms = new_rms;                                                    //      keep that shape
      delete shape;
      shape = random_shape;
    }
    counter++;
  } while (rms > kMinError * 2 && counter < kMaxRandomShapeTries);

  return shape;
}

void Renderer::AdjustShapeSize(Shape *shape) {
  double rms = std::numeric_limits<double>::max();
  size_t counter = 0;

  int max_x = original_image_.GetPixelArray()[0].size();
  int max_y = original_image_.GetPixelArray().size();
  ci::Rectf canvas(top_left_corner_, top_left_corner_ + glm::vec2(max_x, max_y));
  glm::vec2 loc = shape->GetLocation();

  do {
    int old_height = shape->GetHeight();            // saving these
    int old_width = shape->GetWidth();
    ci::ColorA old_color = shape->GetColor();

    std::vector<int> rand_dim = GenerateRandomShapeDimensions(loc);
    shape->SetWidth(rand_dim[0]);
    shape->SetHeight(rand_dim[1]);

    ci::ColorA new_color = CalculateAverageColor(loc, rand_dim);
    shape->SetColor(new_color);
    double new_rms = CalculatePartialRootMeanSquare(shape);

    if (new_rms < rms) {
      rms = new_rms;
    } else {                            // if the old version was better, revert
      shape->SetHeight(old_height);
      shape->SetWidth(old_width);
      shape->SetColor(old_color);
    }
    counter++;
  } while (rms > kMinError && counter < kMaxMutations);
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

  int skip_factor = skip_ * skip_;    // skipping over [skip_] pixels for both row and col means it's 1/[skip_]^2 total pixels

  float avg_red = red_total / (pixels.size() * pixels[0].size() / skip_factor);
  float avg_green = green_total / (pixels.size() * pixels[0].size() / skip_factor);
  float avg_blue = blue_total / (pixels.size() * pixels[0].size() / skip_factor);
  ci::ColorA background_color(avg_red, avg_green, avg_blue, 1);
  return background_color;
}

Shape* Renderer::GenerateRandomShape() {
  int max_x = original_image_.GetPixelArray()[0].size();
  int max_y = original_image_.GetPixelArray().size();

  glm::vec2 loc = GenerateRandomLocation(max_x, max_y);
  std::vector<int> dimensions = GenerateRandomShapeDimensions(loc);
  ci::ColorA color = CalculateAverageColor(loc, dimensions);

  return new Rectangle(loc, dimensions[0], dimensions[1], color);
}

ci::ColorA Renderer::GenerateRandomColor() {
  std::uniform_real_distribution<float> rgb_value(0,1);
  static std::default_random_engine generator;
  return ci::ColorA(rgb_value(generator), rgb_value(generator), rgb_value(generator), kAlpha);
}

ci::ColorA Renderer::CalculateAverageColor(glm::vec2& loc, vector<int>& dim) {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();

  // calculating the bounds for the for-loop: only iterate over the pixels that the shape would cover
  size_t row_start = 0;      // if the shape starts further up than the image
  size_t col_start = 0;      // if the shape starts further left than the image
  if (loc.y > top_left_corner_.y) row_start = loc.y - top_left_corner_.y;      // if the shape's y starts on the image
  if (loc.x > top_left_corner_.x) col_start = loc.x - top_left_corner_.x;      // if the shape's x starts on the image
  size_t row_end = std::min(orig_pixels.size(), ((unsigned int) loc.y + dim[1]));  //the lesser of the image size and the shape size
  size_t col_end = std::min(orig_pixels[0].size(), ((unsigned int) loc.x + dim[0]));

  // calculating the weights for each pixel: the farther from the center, the lower the weight
  glm::vec2 center(loc.x/2, loc.y/2);
  int radius = std::min(dim[0],dim[1]);
  std::vector<float> weights;
  float total_weight = 0;

  for (size_t row = row_start; row < row_end; row+= skip_/2) {
    for (size_t col = col_start; col < col_end; col+= skip_/2) {
      glm::vec2 point(col,row);
      float weight = CalculateDistanceFromCenterWeight(center, point, radius);
      total_weight+= weight;
      weights.push_back(weight);
    }
  }

  // calculating the weighted average RGB values
  float red_total = 0;
  float green_total = 0;
  float blue_total = 0;
  size_t weight_idx = 0;

  for (size_t row = row_start; row < row_end; row+= skip_/2) {
    for (size_t col = col_start; col < col_end; col+= skip_/2) {
      red_total += orig_pixels[row][col].GetRed() * (weights[weight_idx] / total_weight);
      green_total += orig_pixels[row][col].GetGreen() * (weights[weight_idx] / total_weight);
      blue_total += orig_pixels[row][col].GetBlue() * (weights[weight_idx] / total_weight);
      weight_idx++;
    }
  }

  return ci::ColorA(red_total, green_total, blue_total, kAlpha);
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

  if (shapes_.size() > kNumShapes / 2) {           // max dimensions are inversely related to the number of iterations;
    max_width /= 4;                                //   larger shapes to cover broader areas in the beginning, smaller
    max_height /= 4;                               //   shapes for details
  } else if (shapes_.size() > kNumShapes / 5) {
    max_width /= 2.5;
    max_height /= 2.5;
  } else if (shapes_.size() > kNumShapes / 10) {
    max_width /= 1.8;
    max_height /= 1.8;
  } else if (shapes_.size() > kNumShapes / 20) {
    max_width /= 1.3;
    max_height /= 1.3;
  }

  int min_width = max_width * (1 - shapes_.size() / kNumShapes) * 0.6;
  int min_height = max_height * (1 - shapes_.size() / kNumShapes) * 0.6;

  // making sure the shape overlaps the image
  if (loc.x < top_left_corner_.x) { min_width = top_left_corner_.x - loc.x + 5; }  // if the shape starts to the left of the image
  if (loc.y < top_left_corner_.y) { min_height = top_left_corner_.y - loc.y + 5; } // if the shape starts above the image

  // preventing too much of the shape from extending off of the image
  if (loc.x > (top_left_corner_.x + max_dim_) * 0.65) { max_width *= 0.3; }    // if the shape starts too far to the right
  if (loc.y > (top_left_corner_.y + max_dim_) * 0.65) { max_height *= 0.3; }   // if the shape starts too far down

  // making sure that max > min
  if (min_width > max_width) { max_width += min_width - max_width + 5; }
  if (min_height > max_height) { max_height += min_height - max_height + 5; }

  // last check if the shape with the min dimensions still doesn't overlap the image
  if (loc.x + min_width < top_left_corner_.x) { min_width += top_left_corner_.x - (loc.x + min_width) + 5; }
  if (loc.y + min_height < top_left_corner_.y) { min_height += top_left_corner_.y - (loc.y + min_height) + 5; }

  std::uniform_real_distribution<float> width(min_width, max_width);
  std::uniform_real_distribution<float> height(min_height, max_height);
  static std::default_random_engine generator;

  return std::vector<int> {(int)width(generator), (int)height(generator)};
}

double Renderer::CalculatePartialRootMeanSquare(Shape* added_shape) {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
  double total_error = 0;
  glm::vec2 loc = added_shape->GetLocation();

  // calculating the bounds for the for-loop: only iterate over the pixels that the shape would cover
  size_t row_start = 0;     // if the shape starts further up than the image
  size_t col_start = 0;     // if the shape starts further left than the image

  if (loc.y > top_left_corner_.y) row_start = loc.y - top_left_corner_.y;      // if the shape's y starts on the image
  if (loc.x > top_left_corner_.x) col_start = loc.x - top_left_corner_.x;      // if the shape's x starts on the image

  size_t row_end = std::min(orig_pixels.size(), ((unsigned int) loc.y + added_shape->GetHeight())); // the lesser of the image height and the shape height
  size_t col_end = std::min(orig_pixels[0].size(), ((unsigned int) loc.x + added_shape->GetWidth())); // the lesser of the image width and the shape width

  // adding up the total RGBA error
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

double Renderer::CalculateRootMeanSquare(Shape* added_shape) {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
  double total_error = 0;
  glm::vec2 loc = added_shape->GetLocation();

  // calculating the bounds for the for-loop: only iterate over the pixels that the shape would cover
  size_t row_start = 0; // if the shape starts further up than the image
  size_t col_start = 0;

  if (loc.y > top_left_corner_.y) row_start = loc.y - top_left_corner_.y;      // if the shape's y starts on the image
  if (loc.x > top_left_corner_.x) col_start = loc.x - top_left_corner_.x;      // if the shape's x starts on the image

  size_t row_end = std::min(orig_pixels.size(), ((unsigned int) loc.y + added_shape->GetHeight())); // the lesser of the image height and the shape height
  size_t col_end = std::min(orig_pixels[0].size(), ((unsigned int) loc.x + added_shape->GetWidth())); // the lesser of the image width and the shape width

  // adding up the total RGBA error
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
  double rms = total_error / std::pow(((orig_pixels.size() * orig_pixels[0].size())/skip_factor),2);
  return rms;
}

double Renderer::CalculateRootMeanSquare() {
  vector<vector<Pixel>> orig_pixels = original_image_.GetPixelArray();
  vector<vector<Pixel>> new_pixels = generated_image_.GetPixelArray();
  double total_error = 0;

  // adding up the total RGBA error
  for (size_t row = 0; row < orig_pixels.size(); row+=skip_) {
    for (size_t col = 0; col < orig_pixels[0].size(); col+=skip_) {
      total_error += std::pow(new_pixels[row][col].GetRed() - orig_pixels[row][col].GetRed(), 2);
      total_error += std::pow(new_pixels[row][col].GetGreen() - orig_pixels[row][col].GetGreen(), 2);
      total_error += std::pow(new_pixels[row][col].GetBlue() - orig_pixels[row][col].GetBlue(), 2);
    }
  }
  double skip_factor = skip_ * skip_;
  double rms = total_error / std::pow(((orig_pixels.size() * orig_pixels[0].size())/skip_factor),2);
  return rms;
}

float Renderer::CalculateDistanceFromCenterWeight(glm::vec2 &center, glm::vec2 &point, int radius) {
  float dist = glm::distance(center,point);
  if (dist <= 1) return 3;    // if the point is the center or really close to it
  else return std::max(0,(radius - (int)dist));
}

void Renderer::AddShapeToGeneratedImage(Shape *shape) {
  glm::vec2 loc = shape->GetLocation();
  std::vector<std::vector<Pixel>> pixels = generated_image_.GetPixelArray();
  ci::ColorA color = shape->GetColor();

  // calculating the bounds for the for-loop: only iterate over the pixels that the shape would cover
  int row_start = 0; // if the shape starts further up than the image
  int col_start = 0;

  if (loc.y > top_left_corner_.y) row_start = loc.y - top_left_corner_.y;   // if the shape's y starts on the image
  if (loc.x > top_left_corner_.x) col_start = loc.x - top_left_corner_.x;   // if the shape's x starts on the image

  int row_end = 0;
  int col_end = 0;
  if (loc.y + shape->GetHeight() >= top_left_corner_.y + pixels.size()) row_end = pixels.size();  // if the shape ends off of the image
  else row_end = shape->GetHeight() + loc.y - top_left_corner_.y;                                 // if the shape ends on the image

  if (loc.x + shape->GetWidth() >= top_left_corner_.x + pixels[0].size()) col_end = pixels[0].size();
  else col_end = shape->GetWidth() + loc.x - top_left_corner_.x;

  // adding the shape's color to the generated image
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

glm::vec2 Renderer::GetTopLeftCorner() { return top_left_corner_; }

void Renderer::SetGeneratedImage(Image &image) { generated_image_ = image; }
}
}
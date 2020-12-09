#pragma once

#include <core/image.h>
#include <visualizer/shape.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <vector>

namespace geometricprimitives {
namespace visualizer {

class Renderer {
public:
  Renderer();
  Renderer(Image& original);

  void draw();

  /*
   * Generates random shapes until it creates one that has minimal error
   */
  void AddShape();

  /*
   * Sets the original image
   */
  void SetOriginalImage(Image& original);
  void Clear();

  const size_t kNumShapes = 100;
  const float kAlpha = 0.5;

  // adjust for accuracy/speed
  const float kSkipProportion = 0.00015;
  const double kMinError = 0.0000010;              // CHANGE
  const size_t kMaxRandomShapeTries = 30;   // CHANGE
  const size_t kMaxColorChanges = 50;       // CHANGE
  const size_t kMaxMutations = 60;           // CHANGE

private:
  ci::ColorA CalculateBackgroundColor();

  Shape* GenerateRandomShape();
  ci::ColorA GenerateRandomColor();
  ci::ColorA CalculateAverageColor(glm::vec2 loc, vector<int> dim);
  glm::vec2 GenerateRandomLocation(int max_x, int max_y);
  vector<int> GenerateRandomShapeDimensions(glm::vec2& loc);

  double CalculateRootMeanSquare(Shape* added_shape);
  double CalculatePartialRootMeanSquare(Shape* added_shape);
  double CalculateRootMeanSquareFaster(Shape* added_shape);

  void AddShapeToGeneratedImage(Shape* shape);
  Shape* GenerateInitialShape();
  void AdjustShapeSize(Shape* shape);
  void AdjustShapeColor(Shape* shape);

  Image original_image_;
  Image generated_image_;
  std::vector<Shape*> shapes_;
  ci::ColorA background_color_;
  glm::vec2 top_left_corner_ = glm::vec2(50,50);
  int skip_;
  int max_dim_;
};
}
}
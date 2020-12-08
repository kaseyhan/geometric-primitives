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
  void SetOriginalImage(Image& original);
  void Clear();

  const float kMaxDimension = 200;
  const double kMinError = 1;              // CHANGE
  const size_t kMaxRandomShapeTries = 300;   // CHANGE
  const size_t kMaxMutations = 50;           // CHANGE
  const float kAlpha = 0.5;                 // CHANGE?
  const size_t kNumShapes = 200;

private:
  ci::ColorA CalculateBackgroundColor();
  Shape* GenerateRandomShape() const;
  double CalculateRootMeanSquare(Shape* added_shape);
  double CalculatePartialRootMeanSquare(Shape* added_shape);
  void AddShapeToGeneratedImage(Shape* shape);

  Image original_image_;
  Image generated_image_;
  std::vector<Shape*> shapes_;
  ci::ColorA background_color_;
  glm::vec2 top_left_corner_ = glm::vec2(100,100);
};
}
}
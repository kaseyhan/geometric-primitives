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
  //void Render();

  /*
   * Generates random shapes until it creates one that has minimal error
   */
  void AddShape();

  const float kWindowSize = 600;
  const float kMaxDimension = 200;

private:
  const double kMinError = 10;          // CHANGE
  ci::Color8u CalculateBackgroundColor();
  Shape* GenerateRandomShape() const;
  double CalculateRootMeanSquare() const;
  Image original_image_;
  Image generated_image_;
  std::vector<Shape> shapes_;
  ci::Color8u background_color_;
  glm::vec2 top_left_corner_ = glm::vec2(0,0);
};
}
}
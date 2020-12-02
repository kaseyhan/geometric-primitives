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
  void SetOriginalImage(Image& original);
  void Clear();

  const float kWindowSize = 600;
  const float kMaxDimension = 200;
  const double kMinError = 10;          // CHANGE
  const float kAlpha = 0.5;             // CHANGE?

private:
  ci::ColorA CalculateBackgroundColor();
  Shape* GenerateRandomShape() const;
  double CalculateRootMeanSquare() const;
  Image original_image_;
  Image generated_image_;
  std::vector<Shape*> shapes_;
  ci::ColorA background_color_;
  glm::vec2 top_left_corner_ = glm::vec2(0,0);
};
}
}
#pragma once

#include <core/image.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace geometricprimitives {
namespace visualizer {

class Renderer {
public:
  Renderer();
  Renderer(Image& original);

  void draw();

  ci::Color8u CalculateBackgroundColor();
  /*
   * Generates random shapes until it creates one that
   */
  void AddShape();

private:
  double CalculateRootMeanSquare() const;
  Image original_image_;
  Image generated_image_;
};
}
}
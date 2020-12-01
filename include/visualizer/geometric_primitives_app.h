#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <visualizer/renderer.h>

namespace geometricprimitives {
namespace visualizer {

class GeometricPrimitivesApp : public ci::app::App {
public:
  GeometricPrimitivesApp();

  void update() override;
  void draw() override;

  void UploadImage(const char* filename);
  std::vector<unsigned char> decodeOneStep(const char* filename);

  const float kWindowSize = 600;

private:
  Renderer renderer_;

};

}
}
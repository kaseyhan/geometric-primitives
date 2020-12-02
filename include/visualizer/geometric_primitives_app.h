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

  /**
   * Takes the image in the specified file and adds it to the renderer as an Image with a 1D vector of Pixels
   */
  void UploadImage(const char* filename);

  std::vector<unsigned char> DecodeOneStep(const char* file_path);

  const float kWindowSize = 600;
  const char* kFilePath = "default_image.png";

private:
  Renderer renderer_;

};

}
}
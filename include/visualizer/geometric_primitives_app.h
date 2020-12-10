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
   * Takes the image in the specified file and adds it to the renderer as an Image
   */
  void UploadImage(const std::string& filename);

  const float kWindowSize = 400;
  const size_t kNumShapes = 200;
  const std::string kFilePath = "C:\\Users\\kjhan\\Desktop\\cinder_0.9.2_vc2015\\my-projects\\final-project-kaseyhan\\puppy.png";

private:

  /*
   * Uses LodePNG to decode the image at the specified file_path into a 2D vector of Pixels
   */
  std::vector<std::vector<Pixel>> DecodeImage(const std::string& file_path);

  Renderer renderer_;
  size_t counter_;      // number of iterations

};

}
}
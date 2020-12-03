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
  void UploadImage(const std::string& filename);

  std::vector<std::vector<Pixel>> DecodeImage(const std::string& file_path);

  const float kWindowSize = 600;
  const std::string kFilePath = "C:\\Users\\kjhan\\Desktop\\cinder_0.9.2_vc2015\\my-projects\\final-project-kaseyhan\\default_image.png";

private:
  Renderer renderer_;

};

}
}
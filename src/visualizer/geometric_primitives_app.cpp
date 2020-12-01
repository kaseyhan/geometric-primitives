#include <visualizer/geometric_primitives_app.h>
#include <lodepng.h>

namespace geometricprimitives {
namespace visualizer {

GeometricPrimitivesApp::GeometricPrimitivesApp() {
  ci::app::setWindowSize((int) kWindowSize, (int) kWindowSize);
}

void GeometricPrimitivesApp::update() {
  renderer_.AddShape();
}

void GeometricPrimitivesApp::draw() {
  renderer_.draw();
}

void GeometricPrimitivesApp::UploadImage(const char* filename) {
  Image image;
  std::vector<unsigned char> raw_pixels = decodeOneStep(filename);
  for (size_t i = 0; i < raw_pixels.size(); i += 4) {
    Pixel pixel((int)raw_pixels[i], (int)raw_pixels[i+1], (int)raw_pixels[i+2]);

  }
}

std::vector<unsigned char> decodeOneStep(const char* filename) {
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

  //decode
  unsigned error = lodepng::decode(image, width, height, filename);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
  return image;
}

}
}

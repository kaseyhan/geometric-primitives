#include <visualizer/geometric_primitives_app.h>
#include <core/pixel.h>
#include <visualizer/renderer.h>
#include <core/image.h>
#include <lodepng.h>
#pragma warning(disable:4700)


namespace geometricprimitives {
namespace visualizer {

GeometricPrimitivesApp::GeometricPrimitivesApp() {
  ci::app::setWindowSize((int) kWindowSize, (int) kWindowSize);
  UploadImage(kFilePath);
}

void GeometricPrimitivesApp::update() {
  //renderer_.AddShape();
}

void GeometricPrimitivesApp::draw() {
  ci::gl::enableAlphaBlending();
  //renderer_.draw();
}

void GeometricPrimitivesApp::UploadImage(const std::string& file_path) {
  std::vector<unsigned char> raw_pixels = DecodeOneStep(file_path);
  std::vector<Pixel> pixels;
  for (size_t i = 0; i < raw_pixels.size()-4; i += 4) {
    Pixel pixel((float)raw_pixels[i], (float)raw_pixels[i+1], (float)raw_pixels[i+2], (float)raw_pixels[i+3]);      // DIVIDE BY 255?
    pixels.push_back(pixel);
  }
  Image image(pixels);
  renderer_.SetOriginalImage(image);
}

std::vector<unsigned char> GeometricPrimitivesApp::DecodeOneStep(const std::string& file_path) {
  std::vector<unsigned char> pixels; //the raw pixels
  unsigned width, height;

  //decode
  auto error = lodepng::decode(pixels, width, height, file_path);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "pixels", 4 bytes per pixel, ordered RGBARGBA
  return pixels;
}

}
}

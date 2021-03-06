#include <visualizer/geometric_primitives_app.h>
#include <core/pixel.h>
#include <visualizer/renderer.h>
#include <core/image.h>
#include <lodepng.h>
#pragma warning(disable:4700)


namespace geometricprimitives {
namespace visualizer {

GeometricPrimitivesApp::GeometricPrimitivesApp() {
  UploadImage(kFilePath);
  ci::app::setWindowSize((int) kWindowSize, (int) kWindowSize);
  counter_ = 0;
}

void GeometricPrimitivesApp::update() {
  if (counter_ < kNumShapes) {
    renderer_.AddShape();
    counter_++;
  }
}

void GeometricPrimitivesApp::draw() {
  ci::gl::clear(ci::Color8u(0,0,0));
  ci::gl::enableAlphaBlending();
  renderer_.draw();
  glm::vec2 string_pos(380,380);
  ci::gl::drawStringCentered(std::to_string((int)counter_), string_pos, ci::ColorA(1,1,1,1));
}

void GeometricPrimitivesApp::UploadImage(const std::string& file_path) {
  std::vector<std::vector<Pixel>> pixels = DecodeImage(file_path);
  Image image(pixels);
  renderer_.SetOriginalImage(image);
}

std::vector<std::vector<Pixel>> GeometricPrimitivesApp::DecodeImage(const std::string& file_path) {
  std::vector<unsigned char> raw; //the raw pixels
  unsigned width, height;

  //decode
  auto error = lodepng::decode(raw, width, height, file_path);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "pixels", 4 bytes per pixel, ordered RGBARGBA

  std::vector<std::vector<Pixel>> pixels_2d;
  size_t index = 0;

  for (size_t row_idx = 0; row_idx < height; row_idx++) {
    std::vector<Pixel> row;
    for (size_t col_idx = 0; col_idx < width; col_idx++) {
      Pixel pixel((float)raw[index]/255, (float)raw[index+1]/255,
                  (float)raw[index+2]/255, (float)raw[index+3]/255);
      row.push_back(pixel);
      index += 4;
    }
    pixels_2d.push_back(row);
  }

  return pixels_2d;
}

}
}

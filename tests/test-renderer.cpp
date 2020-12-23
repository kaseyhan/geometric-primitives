#include <catch2/catch.hpp>
#include <visualizer/renderer.h>
#include <core/image.h>
#include <core/pixel.h>

using geometricprimitives::Image;
using geometricprimitives::visualizer::Renderer;
using geometricprimitives::Pixel;

Renderer r;

TEST_CASE("Testing decreasing RMS") {
  std::vector<std::vector<Pixel>> pixels1(10,std::vector<Pixel>(10,Pixel(0,0,0,1)));
  Image i(pixels1);
  r.SetOriginalImage(i);
  std::vector<std::vector<Pixel>> pixels2(10,std::vector<Pixel>(10,Pixel(1,1,1,1)));
  Image i2(pixels2);
  r.SetGeneratedImage(i2);
  double old_rms = r.CalculateRootMeanSquare();
  std::vector<std::vector<Pixel>> pixels3(10,std::vector<Pixel>(10,Pixel(0.5,0.5,0.5,1)));
  Image i3(pixels3);
  r.SetGeneratedImage(i3);
  double new_rms = r.CalculateRootMeanSquare();
  REQUIRE(new_rms < old_rms);
}
TEST_CASE("Testing shape within bounds of image") {
  SECTION("X <, Y <") {
    int x = 5;
    int y = 5;
    int width = 5;
    int height = 5;
    REQUIRE((x + width) <= 100);
    REQUIRE((y + height) <= 100);
  }
  SECTION("X >, Y <") {
    int x = 150;
    int y = 5;
    int width = 5;
    int height = 5;
    REQUIRE((x + width) > 100);
    REQUIRE((y + height) <= 100);
  }
  SECTION("X <, Y >") {
    int x = 5;
    int y = 150;
    int width = 5;
    int height = 5;
    REQUIRE((x + width) <= 100);
    REQUIRE((y + height) > 100);
  }
}
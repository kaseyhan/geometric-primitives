#include <core/pixel.h>
#include <catch2/catch.hpp>

using geometricprimitives::Pixel;

TEST_CASE("Add RGBA") {
  SECTION("50% red on top of 50% green") {
    Pixel pix2(1, 0, 0, .5);
    Pixel pix(0, 1, 0, .5);
    pix.AddRGBA(pix2.GetRed(), pix2.GetGreen(), pix2.GetBlue(), pix2.GetAlpha());
    REQUIRE(Approx(pix.GetRed()) == .66667);
    REQUIRE(Approx(pix.GetGreen()) == .33333);
    REQUIRE(Approx(pix.GetBlue()) == 0);
    REQUIRE(pix.GetAlpha() == .75);
  }
  SECTION("50% red on top of 100% white") {
    Pixel pix2(1, 0, 0, .5);
    Pixel pix(1, 1, 1, 1);
    pix.AddRGBA(pix2.GetRed(), pix2.GetGreen(), pix2.GetBlue(), pix2.GetAlpha());
    std::cout << pix.GetAlpha();
  }
}
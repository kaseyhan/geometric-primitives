#include <core/pixel.h>

namespace geometricprimitives {

Pixel::Pixel() {}
Pixel::Pixel(float red, float green, float blue) : red_(red), green_(green), blue_(blue) {}
Pixel::Pixel(float red, float green, float blue, float alpha) : red_(red), green_(green), blue_(blue), alpha_(alpha) {}

float Pixel::GetRed() const { return red_; }
float Pixel::GetGreen() const { return green_; }
float Pixel::GetBlue() const { return blue_; }
float Pixel::GetAlpha() const { return alpha_; }

void Pixel::SetRed(float new_red) { red_ = new_red; }
void Pixel::SetGreen(float new_green) { green_ = new_green; }
void Pixel::SetBlue(float new_blue) { blue_ = new_blue; }
void Pixel::SetAlpha(float new_alpha) { alpha_ = new_alpha; }
void Pixel::SetRGBA(float r, float g, float b, float a) {
  red_ = r;
  green_ = g;
  blue_ = b;
  alpha_ = a;
}

}
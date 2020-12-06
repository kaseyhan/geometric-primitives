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

void Pixel::AddRGBA(float added_red, float added_green, float added_blue, float added_alpha) {
  float new_alpha = 1 - (1 - added_alpha) * (1 - alpha_);

  if (new_alpha > 1.0e-6) {
    red_ = added_red * added_alpha / new_alpha + red_ * alpha_ * (1 - added_alpha) / new_alpha;
    green_ = added_green * added_alpha / new_alpha + green_ * alpha_ * (1 - added_alpha) / new_alpha;
    blue_ = added_blue * added_alpha / new_alpha + blue_ * alpha_ * (1 - added_alpha) / new_alpha;
  }

  alpha_ = new_alpha;
}

}
#include <core/image.h>
#include <core/pixel.h>
#include <vector>

namespace geometricprimitives {

Image::Image() {}
Image::Image(vector<vector<Pixel>> &pix_array) { pixel_array_ = pix_array; }

vector<vector<Pixel>> Image::GetPixelArray() const { return pixel_array_; }

void Image::SetPixelArray(vector<vector<Pixel>> &pix_array) {
  pixel_array_ = pix_array;
}

}
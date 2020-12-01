#include <core/image.h>
#include <core/pixel.h>
#include <vector>

namespace geometricprimitives {

Image::Image() {}
Image::Image(vector<vector<Pixel>> &pix_array) { pixel_array_ = pix_array; }

std::vector<std::vector<Pixel>> Image::GetPixelArray() const { return pixel_array_; }

void Image::AddPixelRow(vector<Pixel> &row) {
  pixel_array_.push_back(row);
}

}
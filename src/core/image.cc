#include <core/image.h>
#include <core/pixel.h>
#include <vector>

namespace geometricprimitives {

Image::Image() {}
Image::Image(vector<Pixel> &pix_array) { pixel_array_ = pix_array; }

std::vector<Pixel> Image::GetPixelArray() const { return pixel_array_; }

void Image::SetPixelArray(vector<Pixel> &pix_array) {
  pixel_array_ = pix_array;
}

}
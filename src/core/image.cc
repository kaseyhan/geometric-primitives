#include <core/image.h>
#include <core/pixel.h>
#include <vector>

namespace geometricprimitives {

Image::Image() {}
std::vector<std::vector<Pixel>> Image::GetPixelArray() const { return pixel_array_; }

}
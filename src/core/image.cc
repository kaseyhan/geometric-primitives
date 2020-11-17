#include <core/image.h>
#include <core/pixel.h>
#include <vector>

namespace geometricprimitives {

std::vector<std::vector<Pixel>> Image::GetPixelArray() const { return pixel_array_; }

}
#pragma once

#include <core/pixel.h>
#include <vector>

namespace geometricprimitives {

using std::vector;

class Image {
public:
  Image();
  Image(vector<vector<Pixel>>& pix_array);

  vector<vector<Pixel>> GetPixelArray() const;

private:
  vector<vector<Pixel>> pixel_array_;
};

}
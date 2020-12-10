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
  void SetPixelArray(vector<vector<Pixel>>& pix_array);

private:
  vector<vector<Pixel>> pixel_array_;
};

}
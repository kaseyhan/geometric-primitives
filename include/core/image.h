#pragma once

#include <core/pixel.h>
#include <vector>

namespace geometricprimitives {

using std::vector;

class Image {
public:
  Image();
  Image(vector<Pixel>& pix_array);

  vector<Pixel> GetPixelArray() const;
  void SetPixelArray(vector<Pixel>& pix_array);

private:
  vector<Pixel> pixel_array_;
};

}
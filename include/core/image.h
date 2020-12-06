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
  void SetPartialPixelArray(vector<vector<Pixel>>& pix_array, int row_start, int row_end, int col_start, int col_end);

private:
  vector<vector<Pixel>> pixel_array_;
};

}
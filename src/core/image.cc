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

void Image::SetPartialPixelArray(vector<vector<Pixel>> &pix_array, int row_start, int row_end, int col_start,
                                 int col_end) {
  for (size_t row = row_start; row < row_end; row++) {
    for (size_t col = col_start; col < col_end; col++) {
      pixel_array_[row][col] = pix_array[row][col];
    }
  }
}

}
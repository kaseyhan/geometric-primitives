#pragma once

#include <core/image.h>
#include <visualizer/shape.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <vector>

namespace geometricprimitives {
namespace visualizer {

class Renderer {
public:
  Renderer();
  Renderer(Image& original);

  void draw();

  /*
   * Generates random shapes until it creates one that has minimal error then adds that shape to the generated image
   */
  void AddShape();

  /*
  * Calculates the root mean square error of the entire image
  */
  double CalculateRootMeanSquare();

  /*
   * Sets the original image
   */
  void SetOriginalImage(Image& original);

  void Clear();
  glm::vec2 GetTopLeftCorner();
  void SetGeneratedImage(Image& image); // for testing purposes

  const size_t kNumShapes = 200;                   // number of shapes to generate
  const float kAlpha = 0.5;                        // alpha value (opacity) of the shapes

  // adjust for accuracy/speed
  const float kSkipProportion = 0.00015;           // proportion of the original image
  const double kMinError = 0.0000010;              // minimum error for a shape to be acceptable
  const size_t kMaxRandomShapeTries = 50;          // maximum number of initial shape generations
  const size_t kMaxMutations = 70;                 // maximum number of shape dimension mutations

private:
  /*
   * Calculates and returns the average color of the entire image for the background.
   */
  ci::ColorA CalculateBackgroundColor();

/*
 * Randomly generates a shape with a random location, random dimensions, and the average color of the image underneath the shape.
 */
  Shape* GenerateRandomShape();

  /*
   * Generates and returns a color with random RGB values
   */
  ci::ColorA GenerateRandomColor();

  /*
   * Calculates the average color of the original image underneath the shape at loc with dim dimensions
   */
  ci::ColorA CalculateAverageColor(glm::vec2& loc, vector<int>& dim);

  /*
   * Generates and returns a vec2 of random x and y coordinates from (0,0) to (max_x,max_y)
   */
  glm::vec2 GenerateRandomLocation(int max_x, int max_y);

  /*
   * Generates and returns a vector of random dimensions (width, height) based on the shape's starting location
   *   (ensures that the shape will overlap with the image)
   */
  vector<int> GenerateRandomShapeDimensions(glm::vec2& loc);

  /*
  * Calculates the root mean square error of the pixels changed by added_shape
  */
  double CalculatePartialRootMeanSquare(Shape* added_shape);

  /*
   * Calculates the root mean square error of the entire image with added_shape
   */
  double CalculateRootMeanSquare(Shape* added_shape);

  /*
   * Updates the generated image by adding the RGBA values of shape to their corresponding pixels
   */
  void AddShapeToGeneratedImage(Shape* shape);

  /*
   * Generates a random initial shape with an "okay" RMSE
   */
  Shape* GenerateInitialShape();

  /*
   * Adjusts the dimensions of shape until it meets the RMSE threshhold
   */
  void AdjustShapeSize(Shape* shape);

  /*
   * Calculates and returns the weight for a point based on its distance from the center
   */
  float CalculateDistanceFromCenterWeight(glm::vec2& center, glm::vec2& point, int radius);

  Image original_image_;
  Image generated_image_;
  std::vector<Shape*> shapes_;
  ci::ColorA background_color_;
  glm::vec2 top_left_corner_ = glm::vec2(50,50);
  int skip_;        // number of pixels to skip over when looping through all pixels
  int max_dim_;     // maximum dimension of generated shapes
};
}
}
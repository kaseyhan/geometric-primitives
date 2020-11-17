#pragma once

namespace geometricprimitives {

class Pixel {
public:
  Pixel();
  Pixel(int red, int green, int blue);

  int GetRed() const;
  int GetGreen() const;
  int GetBlue() const;
  glm::vec2 GetLocation() const;

  void SetRed(int new_red);
  void SetGreen(int new_green);
  void SetBlue(int new_blue);
  void SetLocation(glm::vec2 new_loc);

private:
  int red_;
  int green_;
  int blue_;
  glm::vec2 location_;
};
}
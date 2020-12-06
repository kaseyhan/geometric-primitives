#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("Testing shape within bounds of image") {
  SECTION("X <, Y <") {
    int x = 5;
    int y = 5;
    int width = 5;
    int height = 5;
    REQUIRE((x + width) <= 100);
    REQUIRE((y + height) <= 100);
  }
  SECTION("X >, Y <") {
    int x = 150;
    int y = 5;
    int width = 5;
    int height = 5;
    REQUIRE((x + width) > 100);
    REQUIRE((y + height) <= 100);
  }
  SECTION("X <, Y >") {
    int x = 5;
    int y = 150;
    int width = 5;
    int height = 5;
    REQUIRE((x + width) <= 100);
    REQUIRE((y + height) > 100);
  }
}
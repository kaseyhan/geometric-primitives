#include <visualizer/renderer.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace geometricprimitives {
namespace visualizer {

Renderer::Renderer() {}
Renderer::Renderer(Image& original) : original_image_(original){}

ci::Color8u Renderer::CalculateBackgroundColor() {
  ci::Color8u background_color;
  return background_color;
}

void draw() {

}

}
}
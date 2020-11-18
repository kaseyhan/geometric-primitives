#include <visualizer/geometric_primitives_app.h>

namespace geometricprimitives {
namespace visualizer {

GeometricPrimitivesApp::GeometricPrimitivesApp() {
  ci::app::setWindowSize((int) kWindowSize, (int) kWindowSize);
}

void GeometricPrimitivesApp::update() {
  renderer_.AddShape();
}

void GeometricPrimitivesApp::draw() {
  renderer_.draw();
}
}
}
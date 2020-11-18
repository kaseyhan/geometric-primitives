#include <visualizer/geometric_primitives_app.h>

namespace geometricprimitives {
namespace visualizer {

GeometricPrimitivesApp::GeometricPrimitivesApp() {
  // instantiate renderer
}

void GeometricPrimitivesApp::draw() {
  renderer_.draw();
}
}
}
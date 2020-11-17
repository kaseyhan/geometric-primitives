#include <visualizer/geometric_primitives_app.h>

using geometricprimitives::visualizer::GeometricPrimitivesApp;

void prepareSettings(GeometricPrimitivesApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(GeometricPrimitivesApp, ci::app::RendererGl(ci::app::RendererGl::Options().msaa(4)), prepareSettings);

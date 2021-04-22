#include <fmt/core.h>

#include "abcg.hpp"
#include "openglwindow.hpp"

int main(int argc, char **argv) {
  try {
    // Create application instance
    abcg::Application app(argc, argv);

    // Create OpenGL window
    auto window{std::make_unique<OpenGLWindow>()};
    window->setOpenGLSettings(
        {.samples = 2, .vsync = true, .preserveWebGLDrawingBuffer = true});
    window->setWindowSettings(
        {.width = 1000, .height = 1000, .showFPS = false, .showFullscreenButton = false, .title = "Atividade 1"});

    // Run application
    app.run(window);
  } catch (abcg::Exception &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}
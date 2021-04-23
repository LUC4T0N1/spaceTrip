#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "camera.hpp"
#include "model.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_program{};

  Model m_model;

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_dollySpeed{1.0f};
  float m_truckSpeed{0.0f};
  float m_upSpeed{0.0f};
  float m_downSpeed{0.0f};
  bool m_paused = false;
  bool m_easy = true;


  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void update();
  void changeSpeed(bool easy);
};

#endif
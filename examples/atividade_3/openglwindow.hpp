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

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void update();
  void changeSpeed(bool easy);

    // Light and material properties
  glm::vec4 m_lightDir{0.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f, 0.5f, 1.0f, 1.0f};
  glm::vec4 m_Id{1.0f, 0.5f, 1.0f, 1.0f};
  glm::vec4 m_Is{1.0f, 0.5f, 1.0f, 1.0f};
  glm::vec4 m_Ka{0.1f, 0.1f, 0.1f, 1.0f};
  glm::vec4 m_Kd{0.7f, 0.7f, 0.7f, 1.0f};
  glm::vec4 m_Ks{1.0f, 1.0f, 1.0f, 1.0f};
  float m_shininess{25.0f};
};

#endif
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
  float m_xSpeed{1.0f};
  float m_upSpeed{0.0f};
  float m_zSpeed{1.0f};
  float m_downSpeed{0.0f};
  bool m_paused = false;

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void loadModel(std::string_view path);
  void update();
  void changeLevel(int level);
  void process(glm::mat3 normalMatrix, GLint modelMatrixLoc, glm::mat3 modelViewMatrix, GLint normalMatrixLoc, glm::mat4 model);

    // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f, 0.8f, 1.0f, 1.0f};
  glm::vec4 m_Id{1.0f, 0.8f, 1.0f, 1.0f};
  glm::vec4 m_Is{1.0f, 0.8f, 1.0f, 1.0f};
  glm::vec4 m_Ia2{1.0f, 1.0f, 0.2f, 1.0f};
  glm::vec4 m_Id2{1.0f, 1.0f, 0.2f, 1.0f};
  glm::vec4 m_Is2{1.0f, 1.0f, 0.2f, 1.0f};
  glm::vec4 m_Ia4{1.0f, 0.2f, 0.2f, 1.0f};
  glm::vec4 m_Id4{1.0f, 0.2f, 0.2f, 1.0f};
  glm::vec4 m_Is4{1.0f, 0.2f, 0.2f, 1.0f};
  glm::vec4 m_Ia3{0.2f, 0.9f, 0.2f, 1.0f};
  glm::vec4 m_Id3{0.2f, 0.9f, 0.2f, 1.0f};
  glm::vec4 m_Is3{0.2f, 0.9f, 0.2f, 1.0f};
  glm::vec4 m_Ka{1.0f};
  glm::vec4 m_Kd{1.0f};
  glm::vec4 m_Ks{1.0f};
  float m_shininess{15.0f};
  glm::vec4 m_obs{0.7f, 0.7f, 0.7f, 1.0f};
};

#endif
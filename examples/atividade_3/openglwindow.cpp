#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if(ev.key.keysym.sym == SDLK_r)m_camera.restart();
    if(ev.key.keysym.sym == SDLK_p){
      if(m_paused){
        if(m_easy) m_dollySpeed = 1.0f;
        else m_dollySpeed = 3.0f;
        m_paused = false;
      }else{
        m_dollySpeed = 0.0f;
        m_truckSpeed = 0.0f;
        m_downSpeed = 0.0f;
        m_upSpeed = 0.0f;
        m_paused = true;
      }
    }
    if(!m_paused){
      if(ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) m_truckSpeed = -1.0f;
      if(ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) m_truckSpeed = 1.0f;
      if(ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)  m_downSpeed = 0.5f;
      if(ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)  m_upSpeed = 0.5f;
    }
  }
  if (ev.type == SDL_KEYUP) {
    if(!m_paused){
      if((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a )&& m_truckSpeed < 0) m_truckSpeed = 0.0f;
      if((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d )&& m_truckSpeed > 0) m_truckSpeed = 0.0f;
      if((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) && m_downSpeed > 0)  m_downSpeed = 0.0f;
      if((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) && m_upSpeed > 0)  m_upSpeed = 0.0f;
    }
  }
}

void OpenGLWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "lookat.vert",
                                    getAssetsPath() + "lookat.frag");

  // Load model
  m_model.loadFromFile(getAssetsPath() + "box.obj");

  m_model.setupVAO(m_program);

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}


void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);
  //glBindVertexArray(m_VAO);

  // Get location of uniform variables (could be precomputed)
  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  GLint colorLoc{glGetUniformLocation(m_program, "color")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);

  // Draw white box
  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(20.0f, 2.0f, 0.2f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
  m_model.render();

  // Draw yellow box
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 20.0f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 1.0f, 0.8f, 0.0f, 1.0f);
  m_model.render();

  // Draw blue box
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(20.0f, 2.0f, 0.2f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);
  m_model.render();

  // Draw red box
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 20.0f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 1.0f, 0.25f, 0.25f, 1.0f);
  m_model.render();


    // Draw ultimo obstaculo
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 0.5f, -5.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 0.5f, 0.85f, 1.0f, 1.0f);
  m_model.render();

    // Draw terceiro obstaculo
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 0.5f, 0.85f, 1.0f, 1.0f);
  m_model.render();

    // Draw segundo obstaculo
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 0.5f, 0.85f, 1.0f, 1.0f);
  m_model.render();

      // Draw primeiro obstaculo
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 4.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, 0.5f, 0.85f, 1.0f, 1.0f);
  m_model.render();

  glUseProgram(0);


}

void OpenGLWindow::paintUI() { 
  abcg::OpenGLWindow::paintUI(); 
  {
    auto widgetSize{ImVec2(230, 35)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

   // combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> comboItems{"Easy", "Hard"};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Choose Difficulty",
                            comboItems.at(currentIndex).c_str())) {
        for (auto index : iter::range(comboItems.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      if (currentIndex == 0) {
        changeSpeed(true);
        m_easy = true;
      } else {
        m_easy = false;
       changeSpeed(false);
      }
    }
    ImGui::End();
  }

}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.up(m_upSpeed * deltaTime);
  m_camera.down(m_downSpeed * deltaTime);
  m_camera.verifyCollisions();
}

void OpenGLWindow::changeSpeed(bool easy) {
  if(!m_paused){
    if(easy) m_dollySpeed = 1.0f;
    else m_dollySpeed = 3.0f;
  }
}
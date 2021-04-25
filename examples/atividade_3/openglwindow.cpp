#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <glm/gtc/matrix_inverse.hpp>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if(ev.key.keysym.sym == SDLK_r)m_camera.restart();
    if(ev.key.keysym.sym == SDLK_p){
      if(m_paused){
        m_dollySpeed = 1.0f;
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
      if(ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) m_truckSpeed = -m_xSpeed;
      if(ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) m_truckSpeed = m_xSpeed;
      if(ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)  m_downSpeed = m_zSpeed;
      if(ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)  m_upSpeed = m_zSpeed;
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
  glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "texture.vert",
                                    getAssetsPath() + "texture.frag");

  // Load default model
  loadModel(getAssetsPath() + "box.obj");
  

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::loadModel(std::string_view path) {
  m_model.loadDiffuseTexture(getAssetsPath() + "maps/space.jpg");
  m_model.loadFromFile(path);
  m_model.setupVAO(m_program);
}


void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  GLint normalMatrixLoc{glGetUniformLocation(m_program, "normalMatrix")};
  GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};
  GLint shininessLoc{glGetUniformLocation(m_program, "shininess")};
  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};
  GLint KaLoc{glGetUniformLocation(m_program, "Ka")};
  GLint KdLoc{glGetUniformLocation(m_program, "Kd")};
  GLint KsLoc{glGetUniformLocation(m_program, "Ks")};
  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};
  GLint mappingModeLoc{glGetUniformLocation(m_program, "mappingMode")};


  // Set uniform variables used by every scene object
  glUniformMatrix4fv(viewMatrixLoc, 1,  GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(mappingModeLoc, 2);
  glUniform4fv(lightDirLoc, 1, &m_lightDir.x);
  glUniform1f(shininessLoc, m_shininess);
 
  glUniform4fv(KaLoc, 1, &m_Ka.x);
  glUniform4fv(KdLoc, 1, &m_Kd.x);
  glUniform4fv(KsLoc, 1, &m_Ks.x);

////////////////////////DRAW FIRST LEVEL////////////////////////////////////////////////////

  // Draw left wall 
  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(20.0f, 2.0f, 0.2f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  auto modelViewMatrix{glm::mat3(m_viewMatrix * model)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);
  m_model.render();

  // Draw top wall
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 20.0f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw right wall
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(20.0f, 2.0f, 0.2f));
 process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw bottom wall
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 20.0f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 4
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 0.5f, -5.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

 process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 3
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 2
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

 process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 1
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 4.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);


  //////////////////////////////////////////////SECOND LEVEL//////////////////////////////////////////////////////////////////////

// Draw left wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-1.0f, 5.0f, 0.0f));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(30.0f, 2.0f, 0.2f));
  glUniform4fv(IaLoc, 1, &m_Ia2.x);
  glUniform4fv(IdLoc, 1, &m_Id2.x);
  glUniform4fv(IsLoc, 1, &m_Is2.x);

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);
  // Draw top wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 30.0f));
  
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw right wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 5.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(30.0f, 2.0f, 0.2f));

 process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw bottom wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 30.0f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 8
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 5.5f, -14.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 7
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 4.5f, -11.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 6
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 5.0f, -8.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

 process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 5
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 5.0f, -4.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 4
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 5.5f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 3
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 4.5f, 6.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 2
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 5.0f, 11.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 1
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 5.0f, 15.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

//////////////////////////////////////////////THIRD LEVEL//////////////////////////////////////////////////////////////////////

  // Draw left wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-1.0f, 10.0f, 0.0f));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(40.0f, 2.0f, 0.2f));
  glUniform4fv(IaLoc, 1, &m_Ia3.x);
  glUniform4fv(IdLoc, 1, &m_Id3.x);
  glUniform4fv(IsLoc, 1, &m_Is3.x);
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw top wall
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 11.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 40.0f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw right wall
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 10.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(40.0f, 2.0f, 0.2f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw bottom wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 9.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 40.0f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 11
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 9.5f, -20.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 10
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 10.0f, -16.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 9
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 10.0f, -12.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 8
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 10.5f, -8.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

 process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 7
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 9.5f, -4.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));

  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 6
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 10.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 5
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 10.0f, 4.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 4
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 10.5f, 8.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 3
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 9.5f, 12.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 2
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 10.0f, 16.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 1
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 10.0f, 20.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  //////////////////////////////////////////////FOURTH LEVEL//////////////////////////////////////////////////////////////////////

  // Draw left wall
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-1.0f, 15.0f, 0.0f));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(40.0f, 2.0f, 0.2f));
  glUniform4fv(IaLoc, 1, &m_Ia4.x);
  glUniform4fv(IdLoc, 1, &m_Id4.x);
  glUniform4fv(IsLoc, 1, &m_Is4.x);
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw top walL
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 16.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 40.0f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw right wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 15.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(40.0f, 2.0f, 0.2f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw bottom wall 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 14.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 0.2f, 40.0f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 14
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 15.0f, -21.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 13
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 15.0f, -17.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 12
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 15.5f, -13.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 11
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 14.5f, -9.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 10
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 15.0f, -6.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 9
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 15.0f, -3.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 8
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 15.5f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 7
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 14.5f, 3.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 6
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 15.0f, 6.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 5
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 15.0f, 9.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 4
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 15.5f, 12.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 3
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 14.5f, 15.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 2
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.5f, 15.0f, 18.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  // Draw obstacle 1
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.5f, 15.0f, 21.0f));
  model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
  process(normalMatrix, modelMatrixLoc, modelViewMatrix, normalMatrixLoc, model);

  glUseProgram(0);
}

void OpenGLWindow::paintUI() { 
  abcg::OpenGLWindow::paintUI();
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
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.up(m_upSpeed * deltaTime);
  m_camera.down(m_downSpeed * deltaTime);
  m_camera.verifyCollisions();
  m_camera.verifyFinish();
  int level = m_camera.m_actualLevel;
  changeLevel(level);
}

void OpenGLWindow::changeLevel(int level) {
  if(!m_paused){
    if(level == 1){
    m_dollySpeed = 1.0f;
    m_xSpeed = 1.0f;
    m_zSpeed = 1.0f;
    } 
    else if(level == 2){
    m_dollySpeed = 2.0f;
    m_xSpeed = 2.5f;
    m_zSpeed = 2.5f;
    } 
    else if(level == 3){
      m_dollySpeed = 3.0f;
      m_xSpeed = 4.0f;
      m_zSpeed = 4.0f;
    }else{
      m_dollySpeed = 3.5f;
      m_xSpeed = 5.0f;
      m_zSpeed = 5.0f;
    } 
  }
}

void OpenGLWindow::process(glm::mat3 normalMatrix, GLint modelMatrixLoc, glm::mat3 modelViewMatrix, GLint normalMatrixLoc, glm::mat4 model) {
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  modelViewMatrix = {(m_viewMatrix * model)};
  normalMatrix = {glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
  m_model.render();
}
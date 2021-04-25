#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class OpenGLWindow;

class Camera {
 public:
  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height);

  void dolly(float speed);
  void truck(float speed);
  void up(float speed);
  void down(float speed);
  void restart();
  void verifyCollisions();
  void verifyFinish();
  void changeLevel(int level);


 private:
  friend OpenGLWindow;

  glm::vec3 m_eye{glm::vec3(0.0f, 0.0f, 16.0f)};  // Camera position
  glm::vec3 m_at{glm::vec3(0.0f, 0.0f, 0.0f)};   // Look-at point
  glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};   // "up" direction
  int m_actualLevel = 1;

  // Matrix to change from world space to camera soace
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;
};

#endif
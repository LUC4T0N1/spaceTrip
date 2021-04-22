#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 5.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly(float speed) {
   
       // Compute forward vector (view direction)
      glm::vec3 forward = glm::normalize(m_at - m_eye);

       // Move eye and center forward (speed > 0) or backward (speed < 0)
       m_eye += forward * speed;
      m_at += forward * speed;
      computeViewMatrix();
    
}

void Camera::verifyCollisions() {
  //verify obstacle 1
  if(((m_eye.z <= -5.0f) && (m_eye.z >= -5.1f)) && (m_eye.y >= -0.5f)) {
    restart();
  }else if(((m_eye.z <= -2.0f) && (m_eye.z >= -2.1f)) && (m_eye.y <= 0.5f)) {
    restart();
  }else if(((m_eye.z <= 1.0f) && (m_eye.z >= 0.9f)) && (m_eye.x >= -0.5f)) {
    restart();
  }else if(((m_eye.z <= 4.0f) && (m_eye.z >= 3.9f)) && (m_eye.x <= 0.5f)) {
    restart();
  }
}

void Camera::truck(float speed) {
  if(speed>0){
    if(!(m_eye.x <= 0.75f)){
      speed = 0.0f;
    }
  }else{
    if(!(m_eye.x >= -0.75f)){
      speed = 0.0f;
    }
  }
   
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_at - m_eye);
  // Compute vector to the left
  glm::vec3 left = glm::cross(m_up, forward);

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
  
}

// void Camera::crouch() {
//  // Move eye and center down
//   m_at -= m_up * 1.0f;
//   m_eye -= m_up * 1.0f;
//   computeViewMatrix();
// }

// void Camera::getUp() {
//  // Move eye and center up
//   m_at -= -m_up * 1.0f;
//   m_eye -= -m_up * 1.0f;
//   computeViewMatrix();
// }

void Camera::up(float speed) {
 // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  if(m_eye.y <= 0.8f){
  m_at -= -m_up * speed;
  m_eye -= -m_up * speed;
  computeViewMatrix();
   }else{
   speed = 0.0f;
 }
}

void Camera::down(float speed) {
 // Move eye and center to the left (speed < 0) or to the right (speed > 0)
 if(m_eye.y >= -0.8f){
  m_at -= m_up * speed;
  m_eye -= m_up * speed;
    computeViewMatrix();
 }else{
   speed = 0.0f;
 }
}

 void Camera::restart() {
  m_at.x = 0.0f;
  m_at.y = 0.0f;
  m_at.z = 0.0f;
  m_eye.x = 0.0f;
  m_eye.y = 0.0f;
  m_eye.z = 12.0f;
  computeViewMatrix();

}

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


void Camera::truck(float speed) {
  if(speed>0){
    if(!(m_eye.x <= 0.68f)){
      speed = 0.0f;
    }
  }else{
    if(!(m_eye.x >= -0.68f)){
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

void Camera::up(float speed) {
 // Move eye and center to the left (speed < 0) or to the right (speed > 0)
 //limit the movement level 1
 if(m_eye.y <= 3.0f){
    if(m_eye.y <= 0.7f){
    m_at -= -m_up * speed;
    m_eye -= -m_up * speed;
    computeViewMatrix();
    }else{
    speed = 0.0f;
  }
  //limit the movement level 2
 }else if(m_eye.y >= 3.0f && m_eye.y <= 8.0f){
    if(m_eye.y <= 5.7f){
    m_at -= -m_up * speed;
    m_eye -= -m_up * speed;
    computeViewMatrix();
    }else{
    speed = 0.0f;
  }
  //limit the movement level 3
 }else if(m_eye.y >= 9.0f && m_eye.y <= 13.0f){
    if(m_eye.y <= 10.7f){
    m_at -= -m_up * speed;
    m_eye -= -m_up * speed;
    computeViewMatrix();
    }else{
    speed = 0.0f;
  }
  //limit the movement level 4
 }else if(m_eye.y >= 14.0f){
    if(m_eye.y <= 15.7f){
    m_at -= -m_up * speed;
    m_eye -= -m_up * speed;
    computeViewMatrix();
    }else{
    speed = 0.0f;
  }
 }

}

void Camera::down(float speed) {
 // Move eye and center to the left (speed < 0) or to the right (speed > 0)
 //limit the movement level 1
 if(m_eye.y <= 3.0f){
  if(m_eye.y >= -0.7f){
    m_at -= m_up * speed;
    m_eye -= m_up * speed;
      computeViewMatrix();
  }else{
    speed = 0.0f;
  }
  //limit the movement level 2
 }else if(m_eye.y >= 3.0f && m_eye.y <= 8.0f){
   if(m_eye.y >= 4.3f){
    m_at -= m_up * speed;
    m_eye -= m_up * speed;
      computeViewMatrix();
  }else{
    speed = 0.0f;
  }
  //limit the movement level 3
 }else if(m_eye.y >= 9.0f && m_eye.y <= 13.0f){
   if(m_eye.y >= 9.3f){
    m_at -= m_up * speed;
    m_eye -= m_up * speed;
      computeViewMatrix();
  }else{
    speed = 0.0f;
  }
  //limit the movement level 4
}else if(m_eye.y >= 14.0f){
   if(m_eye.y >= 14.3f){
    m_at -= m_up * speed;
    m_eye -= m_up * speed;
      computeViewMatrix();
  }else{
    speed = 0.0f;
  }
}
}
//restart the game
 void Camera::restart() {
  m_at.x = 0.0f;
  m_at.y = 0.0f;
  m_at.z = 0.0f;
  m_eye.x = 0.0f;
  m_eye.y = 0.0f;
  m_eye.z = 16.0f;
  computeViewMatrix();
  m_actualLevel = 1;
}


void Camera::verifyCollisions() {
  //verify level 1
  if(m_eye.y < 1.0){
    //verify obstacle 4
    if(((m_eye.z <= -5.0f) && (m_eye.z >= -5.2f)) && ((m_eye.y >= -0.5f) && (m_eye.y < 3.0f))) {
      restart();
      //verify obstacle 3
    }else if(((m_eye.z <= -2.0f) && (m_eye.z >= -2.2f)) && (m_eye.y <= 0.5f)) {
      restart();
      //verify obstacle 2
    }else if(((m_eye.z <= 1.0f) && (m_eye.z >= 0.8f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 1
    }else if(((m_eye.z <= 4.0f) && (m_eye.z >= 3.8f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
  }else  //verify level 2
  if(m_eye.y > 3.0 && m_eye.y <= 8.0){
     //verify obstacle 8
    if(((m_eye.z <= -14.0f) && (m_eye.z >= -14.2f)) && ((m_eye.y >= 4.5f) && (m_eye.y < 8.0f))) {
      restart();
      //verify obstacle 7
    }else if(((m_eye.z <= -11.0f) && (m_eye.z >= -11.2f)) && (m_eye.y <= 5.5f)) {
      restart();
      //verify obstacle 6
    }else if(((m_eye.z <= -8.0f) && (m_eye.z >= -8.2f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 5
    }else if(((m_eye.z <= -4.0f) && (m_eye.z >= -4.2f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
    //verify obstacle 4
    else if(((m_eye.z <= 0.0f) && (m_eye.z >= -0.2f)) && ((m_eye.y >= 4.5f) && (m_eye.y < 8.0f))) {
      restart();
      //verify obstacle 3
    }else if(((m_eye.z <= 6.0f) && (m_eye.z >= 5.8f)) && (m_eye.y <= 5.5f)) {
      restart();
      //verify obstacle 2
    }else if(((m_eye.z <= 11.0f) && (m_eye.z >= 10.8f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 1
    }else if(((m_eye.z <= 15.0f) && (m_eye.z >= 14.8f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
  }else  //verify level 3
  if(m_eye.y > 8.5 && m_eye.y <= 13.0){
      //verify obstacle 11
    if(((m_eye.z <= -20.0f) && (m_eye.z >= -20.2f)) && (m_eye.y <= 9.5f)) {
      restart();
      //verify obstacle 10
    }else if(((m_eye.z <= -16.0f) && (m_eye.z >= -16.2f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 9
    }else if(((m_eye.z <= -12.0f) && (m_eye.z >= -12.2f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
     //verify obstacle 8
    else if(((m_eye.z <= -8.0f) && (m_eye.z >= -8.2f)) && ((m_eye.y >= 10.5f))) {
      restart();
      //verify obstacle 7
    }else if(((m_eye.z <= -4.0f) && (m_eye.z >= -4.2f)) && (m_eye.y <= 9.5f)) {
      restart();
      //verify obstacle 6
    }else if(((m_eye.z <= 0.0f) && (m_eye.z >= 0.2f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 5
    }else if(((m_eye.z <= 4.0f) && (m_eye.z >= 3.8f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
    //verify obstacle 4
    else if(((m_eye.z <= 8.0f) && (m_eye.z >= 7.8f)) && ((m_eye.y >= 10.5f))) {
      restart();
      //verify obstacle 3
    }else if(((m_eye.z <= 12.0f) && (m_eye.z >= 11.8f)) && (m_eye.y <= 9.5f)) {
      restart();
      //verify obstacle 2
    }else if(((m_eye.z <= 16.0f) && (m_eye.z >= 15.8f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 1
    }else if(((m_eye.z <= 20.0f) && (m_eye.z >= 19.8f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
  }else  //verify level 4
  if(m_eye.y > 14.0f){
      //verify obstacle 14
    if(((m_eye.z <= -21.0f) && (m_eye.z >= -21.3f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 13
    }else if(((m_eye.z <= -17.0f) && (m_eye.z >= -17.3f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
     //verify obstacle 12
    else if(((m_eye.z <= -13.0f) && (m_eye.z >= -13.3f)) && ((m_eye.y >= 15.5f))) {
      restart();
      //verify obstacle 11
    }else if(((m_eye.z <= -9.0f) && (m_eye.z >= -9.3f)) && (m_eye.y <= 14.5f)) {
      restart();
      //verify obstacle 10
    }else if(((m_eye.z <= -6.0f) && (m_eye.z >= -6.3f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 9
    }else if(((m_eye.z <= -3.0f) && (m_eye.z >= -3.3f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
    //verify obstacle 8
    else if(((m_eye.z <= 0.0f) && (m_eye.z >= -0.3f)) && ((m_eye.y >= 15.5f))) {
      restart();
      //verify obstacle 7
    }else if(((m_eye.z <= 3.0f) && (m_eye.z >= 2.6f)) && (m_eye.y <= 14.5f)) {
      restart();
      //verify obstacle 6
    }else if(((m_eye.z <= 6.0f) && (m_eye.z >= 5.6f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 5
    }else if(((m_eye.z <= 9.0f) && (m_eye.z >= 8.6f)) && (m_eye.x <= 0.5f)) {
      restart();
    } //verify obstacle 4
    else if(((m_eye.z <= 12.0f) && (m_eye.z >= 11.6f)) && ((m_eye.y >= 15.5f))) {
      restart();
      //verify obstacle 3
    }else if(((m_eye.z <= 15.0f) && (m_eye.z >= 14.6f)) && (m_eye.y <= 14.5f)) {
      restart();
      //verify obstacle 2
    }else if(((m_eye.z <= 18.0f) && (m_eye.z >= 17.6f)) && (m_eye.x >= -0.5f)) {
      restart();
      //verify obstacle 1
    }else if(((m_eye.z <= 21.0f) && (m_eye.z >= 20.6f)) && (m_eye.x <= 0.5f)) {
      restart();
    }
  }
  
}


void Camera::verifyFinish() {
  //first level finish line
  if(((m_eye.z <= -12.0f) && (m_eye.z >= -12.5f)) && m_eye.y <= 1.0f) {
    changeLevel(2);
    //second level finish line
  }else if(((m_eye.z <= -20.0f) && (m_eye.z >= -23.5f)) && (m_eye.y >= 3.0f && m_eye.y <= 7.0f)) {
    changeLevel(3);
  }
  //third level finish line
  else if(((m_eye.z <= -25.0f) && (m_eye.z >= -32.5f)) && (m_eye.y >= 7.1f && m_eye.y <= 12.0f)) {
    changeLevel(4);
    //fourth level finish line
  } else if(((m_eye.z <= -32.0f) && (m_eye.z >= -36.5f)) && (m_eye.y >= 13.0f)) {
    restart();
  }
}


//change the values of the camera
void Camera::changeLevel(int level) {
    if(level == 2) {
    m_at.x = 0.0f;
    m_at.y = 4.5f;
    m_at.z = 0.0f;
    m_eye.x = 0.0f;
    m_eye.y = 4.5f;
    m_eye.z = 22.0f;
    computeViewMatrix();
    m_actualLevel = 2;
    }else if(level == 3) {
    m_at.x = 0.0f;
    m_at.y = 9.5f;
    m_at.z = 0.0f;
    m_eye.x = 0.0f;
    m_eye.y = 9.5f;
    m_eye.z = 32.0f;
    m_actualLevel = 3;
    computeViewMatrix();
    }else{
    m_at.x = 0.0f;
    m_at.y = 14.5f;
    m_at.z = 0.0f;
    m_eye.x = 0.0f;
    m_eye.y = 14.5f;
    m_eye.z = 42.0f;
    m_actualLevel = 4;
    computeViewMatrix();
  }
}


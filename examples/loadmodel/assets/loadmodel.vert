#version 410 core

layout(location = 0) in vec3 inPosition;

uniform float angle;

void main() {
  float sinAngle = sin(angle);
  float cosAngle = cos(angle);

                    
  gl_Position = vec4( inPosition.x * cosAngle - inPosition.y * sinAngle,
                      inPosition.x * sinAngle + inPosition.z * cosAngle,
                      inPosition.z , 1.0);                  
}
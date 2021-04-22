#version 410

out vec4 outColor;

void main() {
  float i = 1.0 - gl_FragCoord.z;

  if (gl_FrontFacing) {
    outColor = vec4(0.2, 0.4, 0.3, 1);
  } else {
    outColor = vec4(i, 0, 0, 1);
  }
}
#version 410
layout (location = 0) in vec2 position;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

out vec4 finalVertexPosition;
void main(){
   finalVertexPosition = vec4(position, 0.0, 1.0);
   gl_Position = finalVertexPosition;
}
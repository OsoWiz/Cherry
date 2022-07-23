#version 450

layout(location = 0) in vec3 vecPosition;
layout(location = 1) in vec3 vecColor;

layout(location = 0) out vec3 fragColor;

void main(){

gl_Position = vec4(vecPosition, 1.0);
fragColor = vecColor;

}
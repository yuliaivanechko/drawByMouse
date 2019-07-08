#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 aColor;

uniform mat4 view;
uniform mat4 model;


out vec3 ourColor;

void main()
{
 gl_Position = view * model * vec4(Position, 1.0f);
   ourColor = aColor;
}

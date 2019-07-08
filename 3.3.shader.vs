#version 330 core

layout (location = 0) in vec3 Position;

uniform vec3 aColor;

out vec3 ourColor;

void main()
{
   gl_Position = vec4(Position, 1.0f);
   ourColor = aColor;
}

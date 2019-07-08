#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewC;
uniform mat4 modelC;
uniform vec2 pos1;
uniform vec2 pos2;
uniform float radius;

out vec3 Position;
out vec2 fPos1;
out vec2 fPos2;
out float rad;


void main()
{
    gl_Position = viewC * modelC * vec4(aPos, 1.0);
    vec4 pos1 = vec4(pos1, 0.0, 1.0);
    vec4 pos2 = vec4(pos2, 0.0, 1.0);
    vec4 fragPos1 = viewC * modelC * pos1;
    vec4 fragPos2 = viewC * modelC * pos2;
    fPos1 = vec2(fragPos1.x, fragPos1.y);
    fPos2 = vec2(fragPos2.x, fragPos2.y);
    rad = radius;
    
}

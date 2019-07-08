#version 330 core

in float rad;
in vec2 fPos1;
in vec2 fPos2;
out vec4 fragColor;

vec2 toCoords(vec2 fPos1, vec2 fPos2)
{
    vec2 dot1 = vec2(fPos1.x * 0.5 + 0.5, 0.5 + (fPos1.y * 0.5));
    vec2 dot2 = vec2(fPos2.x * 0.5 + 0.5, 0.5 + (fPos2.y * 0.5));
    return vec2((dot1.x + dot2.x) / 2, ((dot1.y + dot2.y) / 2) + 0.03) ;
}
vec4 circle(vec2 p, vec2 center, float radius)
{
    float d = length(p - center);
    return mix(vec4(1.0, 0.0, 0.0, 1.0), mix(vec4(1.0, 1.0, 0.0, 1.0), mix(vec4(0.0, 0.0, 1.0, 1.0), vec4(0.0, 0.0, 0.0, 0.2), smoothstep(radius * 0.495, radius * 0.695, d)), smoothstep(radius * 0.0, radius * 0.495, d)), smoothstep(radius * 0.0, radius* 0.2, d));
}

vec4 scene(vec2 uv, float t)
{
    return circle(uv, vec2(0, sin(t * 16.0) * (sin(t) * 0.9 + 0.9) * 0.9), rad);
}
void main()
{
    vec2 resol = vec2(800.0, 600.0);
    vec2 coord = gl_FragCoord.xy ;
    vec2 uv = coord / resol;
    uv = uv - vec2(0,0.4);
    uv = uv - toCoords(fPos1, fPos2);
    uv.x *= resol.x / resol.y;
    
    vec4 mainCol = scene(uv, 1);
    fragColor = mainCol;
   
}






















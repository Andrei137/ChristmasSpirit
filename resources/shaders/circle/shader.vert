#version 330 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in float in_Idx;

out vec4 gl_Position;
out vec2 ex_Pos;
out vec3 ex_Color;

uniform mat4 viewShader;
uniform mat4 projectionShader;

const vec2 triangleCoords[3] = vec2[3](
    vec2(0.0f, 2.0f),
    vec2(-sqrt(3), -1.0f),
    vec2(sqrt(3), -1.0f)
);

void main()
{
    gl_Position = projectionShader * viewShader * vec4(in_Position, 0.0f, 1.0f);
    ex_Pos = triangleCoords[int(in_Idx)];
    ex_Color = vec3(0.0f, 0.631f, 0.964f);
}

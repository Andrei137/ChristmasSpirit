#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_Uv;

out vec4 gl_Position;
out vec3 ex_normal;
out vec2 uv;

uniform mat4 viewShader;
uniform mat4 projectionShader;
uniform mat4 model2world;

void main(void)
{
    gl_Position = projectionShader * viewShader * model2world * vec4(in_Position, 1.0f);
	ex_normal = in_Normal;
    uv = in_Uv;
}

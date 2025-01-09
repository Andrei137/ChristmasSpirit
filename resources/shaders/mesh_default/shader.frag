#version 330 core

in vec3 ex_normal;
in vec2 uv;

out vec4 out_Color;

uniform sampler2D textureShader;

void main(void)
{
	out_Color = texture(textureShader, uv);
}

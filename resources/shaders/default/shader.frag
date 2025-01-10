#version 330 core

in vec3 ex_Color;

out vec4 out_Color;

uniform int codCol;

void main(void)
{
    out_Color.xyz = ex_Color;
	out_Color.w = 1.0f;
}

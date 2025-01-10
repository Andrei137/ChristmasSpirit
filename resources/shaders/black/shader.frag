#version 330 core

in vec3 ex_Color;

out vec4 out_Color;

uniform int codCol;

void main(void)
{
    out_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

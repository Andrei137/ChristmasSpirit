#version 330 core

in vec3 ex_Color;

out vec3 out_Color;

uniform int codCol;

void main(void)
{
    out_Color = ex_Color;
}

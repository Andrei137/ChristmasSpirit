#version 330 core

in vec3 ex_Color;

out vec3 out_Color;

uniform int codCol;

void main(void)
{
    switch (codCol)
    {
        case 1:
            out_Color=vec3(0.0, 0.0, 0.0);
            break;
        default:
            out_Color=ex_Color;
    }
}
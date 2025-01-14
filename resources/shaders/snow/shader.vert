#version 330 core

layout (location = 0) in float in_Idx;
layout (location = 1) in mat4 in_Translation;

out vec4 gl_Position;
out vec2 ex_Pos;
out vec3 ex_Color;

uniform mat4 viewShader;
uniform mat4 projectionShader;
uniform float time;
uniform float radius;
uniform mat4 translateToLocation;

const float PI = 3.141592;
int instID;
mat4 matrRot;

const vec2 triangleCoords[3] = vec2[3](
    vec2(    0.0f,  2.0f),
    vec2(-sqrt(3), -1.0f),
    vec2( sqrt(3), -1.0f)
);

mat4 rotateX(float theta)
{
    return mat4(
        1.0,         0.0,        0.0,  0.0,
        0.0,  cos(theta), sin(theta),  0.0,
        0.0, -sin(theta), cos(theta),  0.0,
        0.0,         0.0,        0.0,  1.0
    );
}

mat4 rotateY(float theta)
{
    return mat4(
        cos(theta) ,   0.0, sin(theta), 0.0,
        0.0        ,   1.0,        0.0, 0.0,
        -sin(theta),   0.0, cos(theta), 0.0,
        0.0        ,   0.0,        0.0, 1.0
    );
}

mat4 rotateZ(float theta)
{
    return mat4(
        cos(theta) , sin(theta), 0.0,  0.0,
        -sin(theta), cos(theta), 0.0,  0.0,
        0.0        ,        0.0, 1.0,  0.0,
        0.0        ,        0.0, 0.0,  1.0
    );
}

void main()
{
    float instID = (float(gl_InstanceID) + time) * PI;

    matrRot = rotateX(instID / 10)
            * rotateY(instID / 6)
            * rotateZ(3 * instID / 2);

    gl_Position = projectionShader * viewShader
                * translateToLocation * in_Translation * matrRot
                * vec4(triangleCoords[int(in_Idx)] * radius, 0.0f, 1.0f);

    ex_Pos = triangleCoords[int(in_Idx)];
    ex_Color = vec3(1.f, 1.f, 1.f);
}

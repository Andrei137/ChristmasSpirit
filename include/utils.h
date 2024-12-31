#pragma once

#include <GL/glew.h>

namespace Utils
{
    extern GLfloat
        winWidth,
        winHeight;

    extern float
        dist,
        alpha,
        beta,
        width,
        height;

    void LoadTexture(const char*, GLuint&);
    void ProcessNormalKeys(unsigned char, int, int);
    void ProcessSpecialKeys(int key, int, int);
    void ReshapeWindow(GLint, GLint);
}

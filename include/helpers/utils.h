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

    void LoadTexture(const char* a_photoPath, GLuint& a_texture);
    void ProcessNormalKeys(unsigned char a_key, int, int);
    void ProcessSpecialKeys(int a_key, int, int);
    void ReshapeWindow(GLint a_newWidth, GLint a_newHeight);
}

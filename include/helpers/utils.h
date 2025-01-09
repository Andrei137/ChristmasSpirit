#pragma once

#include <GL/glew.h>
#include <string>

#define FILE_PATH(type, name) FilePath(type, name).c_str()

namespace Utils
{
    extern GLfloat
        winWidth,
        winHeight;

    extern float
        width,
        height;

    extern int
        demoIdx;

    void LoadTexture(const char* a_photoPath, GLuint& a_texture);
    void ProcessNormalKeys(unsigned char a_key, int, int);
    void ProcessSpecialKeys(int a_key, int, int);
    void ReshapeWindow(GLint a_newWidth, GLint a_newHeight);

    std::string DropFileExtension(const std::string& a_file);
    std::string FilePath(const std::string& a_type, const std::string& a_name);
}

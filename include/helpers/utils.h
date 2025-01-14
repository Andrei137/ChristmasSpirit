#pragma once

#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"

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

	extern glm::vec3
		cameraPos,
		cameraOrientation,
		cameraVertical;

    void LoadTexture(const char* a_photoPath, GLuint& a_texture);

    void SetUniformInt(GLuint a_ID, const char* a_name, int a_value);
    void SetUniformMat(GLuint a_ID, const char* a_name, glm::mat4 a_mat);

    std::string DropFileExtension(const std::string& a_file);
    std::string FilePath(const std::string& a_type, const std::string& a_name);

    void ProcessNormalKeys(unsigned char a_key, int, int);
    void ProcessSpecialKeys(int a_key, int, int);
    void ReshapeWindow(GLint a_newWidth, GLint a_newHeight);
}

#pragma once

#include <string>
#include <type_traits>
#include <GL/glew.h>
#include "glm/glm.hpp"

#define FILE_PATH(type, name) FilePath(type, name).c_str()

namespace Utils
{
    extern GLfloat
        winWidth,
        winHeight;

    extern float
        sceneTime,
        width,
        height;

    extern int
        sceneIdx;

	extern glm::vec3
		cameraPos,
		cameraOrientation,
		cameraVertical;

    void LoadTexture(const char* a_photoPath, GLuint& a_texture);

    std::string DropFileExtension(const std::string& a_file);
    std::string FilePath(const std::string& a_type, const std::string& a_name);

    void UpdateCamera(glm::vec3 a_cameraPos, glm::vec3 a_cameraOrientation);
    void UpdatePathTime(const std::string& a_mode = "auto");

    void ProcessNormalKeys(unsigned char a_key, int, int);
    void ProcessKeyUp(unsigned char a_key, int, int);
    void ProcessSpecialKeys(int a_key, int, int);
    void ProcessSpecialKeyUp(int a_key, int, int);
    void ReshapeWindow(GLint a_newWidth, GLint a_newHeight);

    template <typename T>
    void SetUniform(GLuint a_ID, const char* a_name, T a_value)
    {
        GLint location{ glGetUniformLocation(a_ID, a_name) };
        if constexpr (std::is_same_v<T, int>)
        {
            glUniform1i(location, a_value);
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            glUniform1f(location, a_value);
        }
        else if constexpr (std::is_same_v<T, glm::mat4>)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &a_value[0][0]);
        }
        else
        {
            static_assert(std::is_same_v<T, void>, "Unsupported uniform type");
        }
    }
}

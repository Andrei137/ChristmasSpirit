#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

namespace Shaders
{
    extern std::unordered_map<std::string, GLuint>
        shaders;

    extern glm::mat4
        view,
        projection;

    void CreateShaders();
    void SetMVP(glm::mat4, glm::mat4);
    void LoadShader(std::string);
    void DestoryShaders();
}

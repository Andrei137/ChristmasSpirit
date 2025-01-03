#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

namespace Shaders
{
    // <name>: <ProgramId>
    extern std::unordered_map<std::string, GLuint>
        shaders;

    extern glm::mat4
        view,
        proj;

    void Create();
    void SetMVP(glm::mat4 a_view, glm::mat4 a_proj);
    void LoadShader(std::string a_name);
    void Destroy();

    // API to change the current shader
    void SetDefault();
    void SetBlack();
}

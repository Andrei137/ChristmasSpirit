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
    void Destroy();
    void SetMVP(glm::mat4 a_view, glm::mat4 a_proj);
    void SetShader(std::string a_name, glm::mat4 a_custom_view);

    // API to change the current shader
    void SetDefault();
    void SetBlack();
    void SetMeshDefault(std::string a_texture_name);
    void SetCircle(glm::mat4 a_view = glm::mat4(0));
}

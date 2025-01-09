#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

namespace Textures
{
    // <name>: <Texture>
    extern std::unordered_map<std::string, GLuint>
        textures;

    void Create();
    void Destroy();
    GLuint Get(std::string a_name);
}

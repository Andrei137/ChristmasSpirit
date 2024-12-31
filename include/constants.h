#pragma once

#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"

const GLint
    POSX{ 350 },
    POSY{ 200 };

const glm::vec3
    REF{ glm::vec3(0.0f, 0.0f, 0.0f) },
    VERT{ glm::vec3(0.0f, 0.0f, -1.0f) };

const float
    PI{ 3.141592f },
    ZNEAR{ 1 },
    FOV{ 30 };

const std::string
    TITLE{ "ChristmasSpirit" },
    SHADERS_PATH{ "resources/shaders/" },
    PRIMITIVES_SHADER{ SHADERS_PATH + "primitives" };


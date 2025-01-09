#pragma once

#include <cmath>
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
    FOV{ 30 },
    SQRT3{ static_cast<float>(sqrt(3)) };

const std::string
    TITLE{ "ChristmasSpirit" },
    MESHES_PATH{ "resources/meshes" },
    BEZIER_PATH{ "resources/paths" },
    SHADERS_PATH{ "resources/shaders" },
    TEXTURES_PATH{ "resources/textures" };

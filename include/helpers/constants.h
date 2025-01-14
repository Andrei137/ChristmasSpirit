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

const int
    SNOW_COUNT{ 1000 };

const float
    PI{ 3.141592f },
    ZNEAR{ 0.01f },
    FOV{ 30.0f },
    SQRT3{ static_cast<float>(sqrt(3)) },
    SNOW_RADIUS{ 0.031f },
    DELTA_TIME_SNOW{ 0.01f };

const std::string
    TITLE{ "ChristmasSpirit" },
    MESHES_PATH{ "resources/meshes" },
    BEZIER_PATH{ "resources/paths" },
    SCENES_PATH{ "resources/scenes" },
    SHADERS_PATH{ "resources/shaders" },
    TEXTURES_PATH{ "resources/textures" };

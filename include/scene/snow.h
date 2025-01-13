#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <array>
#include "helpers/constants.h"
#include "bezier/path.h"

namespace Snow
{
    const float
        radius{ 0.15f };

    std::array<glm::vec2, 3> GetCoords(const glm::vec2& a_center);
    void CreateVBO(float a_pathLength);
    void UpdateTranslations(const Path& a_path, float a_deltaTime);
    void Draw(glm::mat4 a_translation);
    void DestroyVBO();
}

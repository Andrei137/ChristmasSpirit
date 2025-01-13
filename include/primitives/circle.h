#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <array>
#include "helpers/constants.h"
#include "bezier/path.h"

namespace Circle
{
    const float
        radius{ 0.25f };

    std::array<glm::vec2, 3> GetCoords(const glm::vec2& a_center);
    void CreateVBO(float a_pathLength);
    void UpdateTranslations(const Path& a_path, float a_deltaTime);
    void Draw();
    void DestroyVBO();
}

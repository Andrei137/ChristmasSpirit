#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <array>

namespace Circle
{
    extern GLuint
        VaoId,
        VboId,
        EboId;

    const int
        radius{ 50 };

    std::array<glm::vec2, 3> GetCoords(const glm::vec2& a_center);
    void CreateVBO();
    void Draw();
    void DestroyVBO();
}

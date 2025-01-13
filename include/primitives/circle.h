#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <array>
#include "helpers/constants.h"

namespace Circle
{
    extern GLuint
        VaoId,
        VbPos,
        VbIdx,
        VbTranslationMat,
        EboId;

    extern glm::mat4
        TranslationMat[SNOW_COUNT];

    const int
        radius{ 50 };

    std::array<glm::vec2, 3> GetCoords(const glm::vec2& a_center);
    void CreateVBO();
    void UpdateTranslations();
    void Draw();
    void DestroyVBO();
}

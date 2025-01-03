#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"

namespace Cone
{
    extern GLuint
        VaoId,
        VboId,
        EboId;

    const int
        nr_merid{ 25 },
        radius{ 50 },
        height{ 75 };

    void CreateVBO();
    void Draw();
    void DestroyVBO();
}

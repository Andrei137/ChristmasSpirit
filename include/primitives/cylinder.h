#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"

namespace Cylinder
{
    extern GLuint
        VaoId,
        VboId,
        EboId;

    const int
        nr_merid{ 25 },
        radius{ 35 },
        height{ 100 };

    void CreateVBO();
    void Draw();
    void DestroyVBO();
}

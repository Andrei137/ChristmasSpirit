#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"

namespace Cone
{
    extern GLuint
        VaoId,
        VboId,
        EboId,
        ProgramId,
        ViewLocation,
        ProjLocation,
        CodColLocation;

    const int
        nr_merid{ 25 },
        radius{ 50 },
        height{ 75 };

    void CreateVBO();
    void CreateShaders();
    void Draw(glm::mat4, glm::mat4, int);
    void DestroyShader();
    void DestroyVBO();
}

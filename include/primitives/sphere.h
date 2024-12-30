#pragma once

#include "glm/glm.hpp"
#include "constants.h"

namespace Sphere
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
        nr_parr{ 10 },
        nr_merid{ 20 },
        radius{ 50 };

    const float
        u_min{  -PI / 2 },
        u_max{ PI / 2 },
        v_min{ 0 },
        v_max{ 2 * PI },
        step_u{ (u_max - u_min) / nr_parr },
        step_v{ (v_max - v_min) / nr_merid };

    void CreateVBO();
    void CreateShaders();
    void Draw(glm::mat4, glm::mat4, int);
    void DestroyShader();
    void DestroyVBO();
}

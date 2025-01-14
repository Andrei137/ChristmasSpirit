#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "helpers/constants.h"
#include "bezier/path.h"

namespace Snow
{
    void CreateVBO(Path a_path, const char* a_file);
    void UpdateTranslations();
    void Draw();
    void DestroyVBO();
}

#include "scene/snow.h"
#include <GL/freeglut.h>
#include "helpers/shaders.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cstdlib>

namespace Snow
{
    GLuint
        VaoId,
        VbPos,
        VbIdx,
        VbTranslationMat,
        EboId;

    float
        timeDisplacements[SNOW_COUNT];

    glm::vec2
        positionDisplacements[SNOW_COUNT];

    glm::mat4
        translationMat[SNOW_COUNT];

    std::array<glm::vec2, 3> GetCoords(const glm::vec2& a_center)
    {
        glm::vec2 top{ a_center.x, a_center.y + 2 * radius };
        glm::vec2 left_bottom{ a_center.x - SQRT3 * radius, a_center.y - radius };
        glm::vec2 right_bottom{ a_center.x + SQRT3 * radius, a_center.y - radius };
        return {
            top,
            left_bottom,
            right_bottom
        };
    }

    void ResetLocation(int a_instID)
    {
        positionDisplacements[a_instID].x = (rand() / (float)RAND_MAX - 0.5f) * 6.f;
        positionDisplacements[a_instID].y = (rand() / (float)RAND_MAX - 0.5f) * 6.f;
    }

    void UpdateTimeDisplacements(float a_pathLength, float a_deltaTime, int a_instID)
    {
        timeDisplacements[a_instID] += a_deltaTime;
        if (timeDisplacements[a_instID] >= a_pathLength)
        {
            timeDisplacements[a_instID] -= a_pathLength;
            ResetLocation(a_instID);
        }
    }

    void CreateVBO(float pathLength)
    {
        std::array<glm::vec2, 3> coords{ GetCoords({ 0, 0 }) };
        int currIdx{ 0 };
        static GLfloat Vertices[6];
        static GLfloat Idx[3];
        static GLint Indices[3];

        for (int i = 0; i < 3; ++i)
        {
            Vertices[currIdx++] = coords[i].x;
            Vertices[currIdx++] = coords[i].y;
            Idx[i] = static_cast<float>(i);
            Indices[i] = i;
        }

        for (int i = 0; i < SNOW_COUNT; ++i)
        {
            ResetLocation(i);
            timeDisplacements[i] = (rand() / (float)RAND_MAX) * pathLength;
        }

        glGenVertexArrays(1, &VaoId);
        glBindVertexArray(VaoId);

        glGenBuffers(1, &VbPos);
        glBindBuffer(GL_ARRAY_BUFFER, VbPos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

        glGenBuffers(1, &VbIdx);
        glBindBuffer(GL_ARRAY_BUFFER, VbIdx);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Idx), Idx, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);

        glGenBuffers(1, &VbTranslationMat);
        glBindBuffer(GL_ARRAY_BUFFER, VbTranslationMat);
        glBufferData(GL_ARRAY_BUFFER, sizeof(translationMat), translationMat, GL_DYNAMIC_DRAW);
        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(2 + i);
            glVertexAttribPointer(2 + i,
                4, GL_FLOAT, GL_FALSE,
                sizeof(glm::mat4),
                (void*)(sizeof(glm::vec4) * i));
            glVertexAttribDivisor(2 + i, 1);
        }

        glGenBuffers(1, &EboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    }

    void UpdateTranslations(const Path& a_path, float a_deltaTime)
    {
        float pathLength{ a_path.length() };
        for (int instID = 0; instID < SNOW_COUNT; ++instID)
        {
            translationMat[instID] = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(
                    positionDisplacements[instID].x,
                    positionDisplacements[instID].y,
                    0.0
                ) + (a_path.interpolate(timeDisplacements[instID]))
            );
            UpdateTimeDisplacements(pathLength, a_deltaTime, instID);
        }
        glBindBuffer(GL_ARRAY_BUFFER, VbTranslationMat);
        glBufferData(GL_ARRAY_BUFFER, sizeof(translationMat), translationMat, GL_DYNAMIC_DRAW);
    }

    void Draw(glm::mat4 a_translation)
    {
        Shaders::SetCircle(a_translation);
        glBindVertexArray(VaoId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
        glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, 35);
    }

    void DestroyVBO()
    {
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VbPos);
        glDeleteBuffers(1, &VbIdx);
        glDeleteBuffers(1, &VbTranslationMat);
        glDeleteBuffers(1, &EboId);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VaoId);
    }
}

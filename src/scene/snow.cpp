#include "scene/snow.h"
#include <GL/freeglut.h>
#include "helpers/shaders.h"
#include "helpers/constants.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cstdlib>
#include <array>
#include <vector>
#include <fstream>

namespace Snow
{
    GLuint
        VaoId,
        VbIdx,
        VbTranslationMat,
        EboId;

    float
        pathLength,
        timeDisplacements[SNOW_COUNT];

    glm::vec2
        positionDisplacements[SNOW_COUNT];

    std::vector<glm::vec3>
        translations;

    glm::mat4
        translationMat[SNOW_COUNT];

    Path
        snowPath;

    void ReadTranslations(const char* a_file)
    {
        translations.clear();

        std::ifstream in(a_file);

        int nrTranslations;
        in >> nrTranslations;
        translations.reserve(nrTranslations);

        float tx, ty, tz;
        for (int i = 0; i < nrTranslations; ++i)
        {
            in >> tx >> ty >> tz;
            translations.push_back(glm::vec3(tx, ty, tz));
        }
    }

    void ResetDisplacements(int a_instID)
    {
        positionDisplacements[a_instID].x = (rand() / (float)RAND_MAX - 0.5f) * 9.f;
        positionDisplacements[a_instID].y = (rand() / (float)RAND_MAX - 0.5f) * 9.f;
        timeDisplacements[a_instID] -= pathLength;
    }

    void UpdateTimeDisplacements(int a_instID)
    {
        timeDisplacements[a_instID] += DELTA_TIME_SNOW;
        if (timeDisplacements[a_instID] >= pathLength)
        {
            ResetDisplacements(a_instID);
        }
    }

    void CreateVBO(Path a_path, const char* a_file)
    {
        ReadTranslations(a_file);
        snowPath = a_path;
        pathLength = a_path.length();

        static GLfloat Idx[3] = { 0.0f, 1.0f, 2.0f };
        static GLint Indices[3] = { 0, 1, 2 };

        for (int instID = 0; instID < SNOW_COUNT; ++instID)
        {
            ResetDisplacements(instID);
            timeDisplacements[instID] = (rand() / (float)RAND_MAX) * pathLength;
        }

        glGenVertexArrays(1, &VaoId);
        glBindVertexArray(VaoId);

        glGenBuffers(1, &VbIdx);
        glBindBuffer(GL_ARRAY_BUFFER, VbIdx);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Idx), Idx, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);

        glGenBuffers(1, &VbTranslationMat);
        glBindBuffer(GL_ARRAY_BUFFER, VbTranslationMat);
        glBufferData(GL_ARRAY_BUFFER, sizeof(translationMat), translationMat, GL_DYNAMIC_DRAW);
        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i,
                4, GL_FLOAT, GL_FALSE,
                sizeof(glm::mat4),
                (void*)(sizeof(glm::vec4) * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        glGenBuffers(1, &EboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    }

    void UpdateTranslations()
    {
        for (int instID = 0; instID < SNOW_COUNT; ++instID)
        {
            translationMat[instID] = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(
                    positionDisplacements[instID].x,
                    positionDisplacements[instID].y,
                    0.0
                ) + (snowPath.interpolate(timeDisplacements[instID]))
            );
            UpdateTimeDisplacements(instID);
        }
        glBindBuffer(GL_ARRAY_BUFFER, VbTranslationMat);
        glBufferData(GL_ARRAY_BUFFER, sizeof(translationMat), translationMat, GL_DYNAMIC_DRAW);
    }

    void Draw()
    {
        glBindVertexArray(VaoId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

        for (const glm::vec3& translation : translations) {
            Shaders::SetSnow(glm::translate(glm::mat4(1.0f), translation));
            glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, SNOW_COUNT);
        }
    }

    void DestroyVBO()
    {
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VbIdx);
        glDeleteBuffers(1, &VbTranslationMat);
        glDeleteBuffers(1, &EboId);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VaoId);
    }
}

#include "primitives/circle.h"
#include <GL/freeglut.h>
#include "helpers/shaders.h"
#include "helpers/constants.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Circle
{
    GLuint
        VaoId,
        VboId;

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

    void CreateVBO()
    {
        std::array<glm::vec2, 3> coords{ GetCoords({ 0, 0 }) };
        int currIdx{ 0 };
        static GLfloat Vertices[12];

        for (int i = 0; i < 3; ++i)
        {
            Vertices[currIdx++] = coords[i].x;
            Vertices[currIdx++] = coords[i].y;
            Vertices[currIdx++] = static_cast<float>(i);
        }

        glGenVertexArrays(1, &VaoId);
        glBindVertexArray(VaoId);
        glGenBuffers(1, &VboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    }

    void Draw()
    {
        Shaders::SetCircle(glm::lookAt(glm::vec3(REF.x + 0.01f, REF.y, REF.z + 2000.0f), REF, VERT));
        glBindVertexArray(VaoId);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void DestroyVBO()
    {
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VboId);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VaoId);
    }
}

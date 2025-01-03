#include "primitives/cone.h"
#include "helpers/constants.h"
#include <GL/freeglut.h>

namespace Cone
{
    GLuint
        VaoId,
        VboId,
        EboId;

    void CreateVBO(void)
    {
        glm::vec4 Vertices[nr_merid + 1];
        glm::vec3 Colors[nr_merid + 1];
        GLushort Indices[4 * nr_merid];

        for (int i = 0; i < nr_merid; ++i)
        {
            float u{ 2 * PI * i / nr_merid };
            float x{ radius * cosf(u) };
            float y{ radius * sinf(u) };
            float z{ -radius };

            Vertices[i] = glm::vec4(x, y, z, 1.0);
            Colors[i] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(u), 0.1f + -1.5 * sinf(u));
            Indices[i] = i;
        }

        Vertices[nr_merid] = glm::vec4(0, 0, height, 1);
        Colors[nr_merid] = glm::vec3(0.2, 0.2, 0.2);

        for (int i = 0; i < nr_merid; ++i)
        {
            Indices[nr_merid + 3 * i] = nr_merid;
            Indices[nr_merid + 3 * i + 1] = i;
            Indices[nr_merid + 3 * i + 2] = (i + 1) % nr_merid;
        }

        glGenVertexArrays(1, &VaoId);
        glBindVertexArray(VaoId);
        glGenBuffers(1, &VboId);
        glGenBuffers(1, &EboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices) + sizeof(Colors), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices), sizeof(Colors), Colors);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)sizeof(Vertices));
    }

    void Draw()
    {
        glBindVertexArray(VaoId);

        // Base
        glDrawElements(
            GL_TRIANGLE_FAN,
            nr_merid,
            GL_UNSIGNED_SHORT,
            0
        );

        // Lateral faces
        for (int i = 0; i < nr_merid; ++i)
        {
            glDrawElements(
                GL_TRIANGLES,
                3,
                GL_UNSIGNED_SHORT,
                (void*)(sizeof(GLushort) * (nr_merid + 3 * i))
            );
        }
    }

    void DestroyVBO()
    {
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VboId);
        glDeleteBuffers(1, &EboId);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VaoId);
    }
}

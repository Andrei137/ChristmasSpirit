#include "primitives/cylinder.h"
#include "helpers/constants.h"
#include <GL/freeglut.h>

namespace Cylinder
{
    GLuint
        VaoId,
        VboId,
        EboId;

    void CreateVBO(void)
    {
        glm::vec4 Vertices[nr_merid * 2];
        glm::vec3 Colors[nr_merid * 2];
        GLushort Indices[nr_merid * 6];

        for (int merid = 0; merid < nr_merid; ++merid)
        {
            float u{ 2 * PI * merid / nr_merid };
            float x{ radius * cosf(u) };
            float y{ radius * sinf(u) };
            float z{ -radius };

            Vertices[merid] = glm::vec4(x, y, z, 1.0);
            Colors[merid] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(u), 0.1f + -1.5 * sinf(u));
            Indices[merid] = merid;

            Vertices[nr_merid + merid] = glm::vec4(x, y, z + height, 1.0);
            Colors[nr_merid + merid] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(u), 0.1f + -1.5 * sinf(u));
            Indices[nr_merid + merid] = nr_merid + merid;

            Indices[2 * nr_merid + 4 * merid] = merid;
            Indices[2 * nr_merid + 4 * merid + 1] = nr_merid + merid;
            Indices[2 * nr_merid + 4 * merid + 2] = nr_merid + merid + 1;
            Indices[2 * nr_merid + 4 * merid + 3] = merid + 1;
        };

        Indices[6 * nr_merid - 2] = nr_merid;
        Indices[6 * nr_merid - 1] = 0;

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

        // Superior Base
        glDrawElements(
            GL_TRIANGLE_FAN,
            nr_merid,
            GL_UNSIGNED_SHORT,
            0
        );

        // Inferior Base
        glDrawElements(
            GL_TRIANGLE_FAN,
            nr_merid,
            GL_UNSIGNED_SHORT,
            (GLvoid*)(nr_merid * sizeof(GLushort))
        );

        // Lateral Faces
        glDrawElements(
            GL_QUADS,
            4 * nr_merid,
            GL_UNSIGNED_SHORT,
            (GLvoid*)(2 * nr_merid * sizeof(GLushort))
        );
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

#include "primitives/sphere.h"
#include <GL/freeglut.h>

namespace Sphere
{
    GLuint
        VaoId,
        VboId,
        EboId;

    void CreateVBO(void)
    {
        glm::vec4 Vertices[(nr_parr + 1) * nr_merid];
        glm::vec3 Colors[(nr_parr + 1) * nr_merid];
        GLushort Indices[2 * (nr_parr + 1) * nr_merid + 4 * (nr_parr + 1) * nr_merid];

        for (int merid = 0; merid < nr_merid; ++merid)
        {
            for (int parr = 0; parr < nr_parr + 1; ++parr)
            {
                float u{ u_min + parr * step_u };
                float v{ v_min + merid * step_v };
                float x{ radius * cosf(u) * cosf(v) };
                float y{ radius * cosf(u) * sinf(v) };
                float z{ radius * sinf(u) };

                int index{ merid * (nr_parr + 1) + parr };
                Vertices[index] = glm::vec4(x, y, z, 1.0);
                Colors[index] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(v), 0.1f + -1.5 * sinf(u));
                Indices[index] = index;

                int index_aux{ parr * (nr_merid) + merid };
                Indices[(nr_parr + 1) * nr_merid + index_aux] = index;

                if ((parr + 1) % (nr_parr + 1) != 0)
                {
                    int aux{ 2 * (nr_parr + 1) * nr_merid };
                    int index1{ index };
                    int index2{ index + (nr_parr + 1) };
                    int index3{ index2 + 1 };
                    int index4{ index + 1 };
                    if (merid == nr_merid - 1)
                    {
                        index2 = index2 % (nr_parr + 1);
                        index3 = index3 % (nr_parr + 1);
                    }
                    Indices[aux + 4 * index] = index1;
                    Indices[aux + 4 * index + 1] = index2;
                    Indices[aux + 4 * index + 2] = index3;
                    Indices[aux + 4 * index + 3] = index4;
                }
            }
        };

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

        for (int patr = 0; patr < (Sphere::nr_parr + 1) * Sphere::nr_merid; ++patr)
        {
            if ((patr + 1) % (Sphere::nr_parr + 1) != 0)
            {
                glDrawElements(
                    GL_QUADS,
                    4,
                    GL_UNSIGNED_SHORT,
                    (GLvoid*)((2 * (Sphere::nr_parr + 1) * (Sphere::nr_merid) + 4 * patr) * sizeof(GLushort))
                );
            }
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

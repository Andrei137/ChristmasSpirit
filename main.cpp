#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "SOIL.h"
#include "loadShaders.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

/* Variables Section */
GLuint
    VaoId,
    VboId,
    EboId,
    ProgramId,
    viewLocation,
    projLocation,
    codColLocation;
GLfloat
    winWidth = 600,
    winHeight = 600;
glm::vec3
    obs,
    pctRef,
    vert;
glm::mat4
    view,
    projection;
int codCol;
float
    PI = 3.141592,
    obsX = 0.0,
    obsY = 0.0,
    obsZ = 300.f,
    refX = 0.0f,
    refY = 0.0f,
    refZ = -100.f,
    vX = 0.0,
    xMin = -300.f,
    xMax = 300.f,
    yMin = -300.f,
    yMax = 300.f,
    dNear = 100.f,
    dFar = 500.f,
    width = 600.f,
    height = 600.f,
    fov = 90.f * PI / 180;

/* Initialization Section */
void LoadTexture(const char* a_photoPath, GLuint& a_texture)
{
    glGenTextures(1, &a_texture);
    glBindTexture(GL_TEXTURE_2D, a_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int tex_width{}, tex_height{};
    unsigned char* image{ SOIL_load_image(a_photoPath, &tex_width, &tex_height, 0, SOIL_LOAD_RGBA) };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CreateShaders(void)
{
    ProgramId = LoadShaders("shaders/01.vert", "shaders/01.frag");
    glUseProgram(ProgramId);
}

void CreateVBO(void)
{
    GLfloat Vertices[] =
    {
        //  Coordonatele bazei
        -75.0f, -75.0f, -45.0f, 1.0f,   0.956f, 0.541f, 0.113f,
         75.0f, -75.0f, -45.0f, 1.0f,   0.470f, 0.007f, 0.850f,
         75.0f,  75.0f, -45.0f, 1.0f,   0.998f, 0.866f, 0.007f,
        -75.0f,  75.0f, -45.0f, 1.0f,   0.160f, 0.796f, 0.180f,

        //  Coordonatele varfului
         0.0f,   0.0f,  45.0f, 1.0f,   1.0f, 1.0f, 1.0f,
    };

    GLubyte Indices[] =
    {
        0, 1, 2, 3,     //  Baza
        0, 3, 4,        //  Fata din stanga
        3, 2, 4,        //  Fata din spate
        1, 2, 4,        //  Fata din dreapta
        0, 1, 4,        //  Fata din fata

        0, 1, 2, 3,     //  Conturul bazei
        0, 4, 1, 4, 2, 4, 3, 4
    };

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
}

void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    CreateVBO();
    CreateShaders();
    viewLocation = glGetUniformLocation(ProgramId, "view");
    projLocation = glGetUniformLocation(ProgramId, "projection");
    codColLocation = glGetUniformLocation(ProgramId, "codCol");
}

/* Render Section */
void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

    obs = glm::vec3(obsX, obsY, obsZ);
    refX = obsX; refY = obsY;
    pctRef = glm::vec3(refX, refY, refZ);
    vert = glm::vec3(vX, 1.0f, 0.0f);
    view = glm::lookAt(obs, pctRef, vert);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


    projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), dNear, dFar);
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

    codCol = 0;
    glUniform1i(codColLocation, codCol);

    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, (void*)(0));

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (void*)(4));

    codCol = 1;
    glUniform1i(codColLocation, codCol);
    glLineWidth(3.5);

    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(16));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(20));

    glutSwapBuffers();
    glFlush();
}

/* Cleanup Section */
void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void DestroyVBO(void)
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VboId);
    glDeleteBuffers(1, &EboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

void Cleanup(void)
{
    DestroyShaders();
    DestroyVBO();
}

/* Main Section */
void ProcessNormalKeys(unsigned char key, int, int)
{
    switch (key)
    {
        case 'l':
            vX += 0.1;
            break;
        case 'r':
            vX -= 0.1;
            break;
        case '+':
            obsZ += 10;
            break;
        case '-':
            obsZ -= 10;
            break;
        }
    if (key == 27)
    {
        exit(0);
    }
}

void ProcessSpecialKeys(int key, int, int)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            obsX -= 5;
            break;
        case GLUT_KEY_RIGHT:
            obsX += 5;
            break;
        case GLUT_KEY_UP:
            obsY += 5;
            break;
        case GLUT_KEY_DOWN:
            obsY -= 5;
            break;
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Christmas Spirit");

    glewInit();

    Initialize();
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(RenderFunction);
    glutKeyboardFunc(ProcessNormalKeys);
    glutSpecialFunc(ProcessSpecialKeys);
    glutCloseFunc(Cleanup);

    glutMainLoop();

    return 0;
}

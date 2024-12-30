#include <windows.h>
#include <stack>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "primitives/cone.h"
#include "primitives/cylinder.h"
#include "primitives/sphere.h"
#include "loadShaders.h"
#include "constants.h"
#include "utils.h"

using namespace Utils;

/* Variables Section */
glm::mat4
    projection;
std::stack<glm::mat4>
    viewStack;

/* Initialization Section */
void CreateShaders(void)
{
    Cone::CreateShaders();
    Cylinder::CreateShaders();
    Sphere::CreateShaders();
}

void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    CreateShaders();
    Cone::CreateVBO();
    Cylinder::CreateVBO();
    Sphere::CreateVBO();
}

void SetMVP()
{
    glm::vec3 obs = {
        REF.x + dist * cos(alpha) * cos(beta),
        REF.y + dist * cos(alpha) * sin(beta),
        REF.z + dist * sin(alpha)
    };

    projection = glm::infinitePerspective(FOV, GLfloat(width) / GLfloat(height), ZNEAR);

    while (!viewStack.empty())
    {
        viewStack.pop();
    }
    viewStack.push(glm::lookAt(obs, REF, VERT));
}

/* Cleanup Section */
void DestroyShaders(void)
{
    Cone::DestroyShader();
    Cylinder::DestroyShader();
    Sphere::DestroyShader();
}

void Cleanup(void)
{
    DestroyShaders();
    Cone::DestroyVBO();
    Cylinder::DestroyVBO();
    Sphere::DestroyVBO();
}

/* Main Section */
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    SetMVP();

    int timeElapsed{ static_cast<int>(glutGet(GLUT_ELAPSED_TIME)) };
    int interval{ 3000 };
    int shapeIdx{ (timeElapsed / interval) % 3 };

    if (shapeIdx == 0)
    {
        Cone::Draw(viewStack.top(), projection, 0);
    }
    else if (shapeIdx == 1)
    {
        Cylinder::Draw(viewStack.top(), projection, 0);
    }
    else
    {
        Sphere::Draw(viewStack.top(), projection, 0);
    }

    glutSwapBuffers();
    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(POSX, POSY);
    glutCreateWindow(TITLE.c_str());

    glewInit();

    Initialize();
    glutReshapeFunc(ReshapeWindow);
    glutDisplayFunc(RenderScene);
    glutIdleFunc(RenderScene);
    glutKeyboardFunc(ProcessNormalKeys);
    glutSpecialFunc(ProcessSpecialKeys);
    glutCloseFunc(Cleanup);

    glutMainLoop();

    return 0;
}
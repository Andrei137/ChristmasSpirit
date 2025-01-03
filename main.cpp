#include <stack>
#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "bezier/path.h"
#include "helpers/constants.h"
#include "helpers/shaders.h"
#include "helpers/utils.h"
#include "primitives/cone.h"
#include "primitives/cylinder.h"
#include "primitives/sphere.h"

using namespace Utils;

/* Variables Section */
double
    time{ 0.0f };
Path
    path{ Path::readFromFile("resources/paths/demo_0.dat") };
glm::mat4
    projection;
std::stack<glm::mat4>
    viewStack;

/* Initialization Section */
void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    Shaders::Create();
    Cone::CreateVBO();
    Cylinder::CreateVBO();
    Sphere::CreateVBO();
}

void SetMVP()
{
	glm::vec3 obs{ path.interpolate(time) };
    time += time < 8.9
        ? 0.01
        : -time;

    projection = glm::infinitePerspective(FOV, GLfloat(width) / GLfloat(height), ZNEAR);

    while (!viewStack.empty())
    {
        viewStack.pop();
    }
    viewStack.push(glm::lookAt(obs, REF, VERT));

    Shaders::SetMVP(viewStack.top(), projection);
}

/* Cleanup Section */
void Cleanup(void)
{
    Shaders::Destroy();
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

    int shaderInterval{ 750 };
    int shaderType{ (timeElapsed / shaderInterval) % 2 };
    if (shaderType == 0)
    {
        Shaders::SetDefault();
    }
    else
    {
        Shaders::SetBlack();
    }

    int shapeInterval{ 3000 };
    int shapeIdx{ (timeElapsed / shapeInterval) % 3 };
    if (shapeIdx == 0)
    {
        Cone::Draw();
    }
    else if (shapeIdx == 1)
    {
        Cylinder::Draw();
    }
    else
    {
        Sphere::Draw();
    }

    glutSwapBuffers();
    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(static_cast<int>(winWidth), static_cast<int>(winHeight));
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

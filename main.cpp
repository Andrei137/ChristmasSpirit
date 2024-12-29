#include <windows.h>
#include <stack>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "loadShaders.h"
#include "Constants.h"
#include "DrawModes.h"
#include "Sphere.h"
#include "Utils.h"

using namespace Utils;

/* Variables Section */
glm::mat4
    projection;
std::stack<glm::mat4>
    viewStack;

/* Initialization Section */
void CreateShaders(void)
{
    Sphere::CreateShaders();
}

void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    CreateShaders();
    Sphere::CreateVBO();
}

void LoadMatrices()
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
    Sphere::DestroyShader();
}

void Cleanup(void)
{
    DestroyShaders();
    Sphere::DestroyVBO();
}

/* Main Section */
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    LoadMatrices();

    Sphere::Draw(viewStack.top(), projection, Primitive::FULL_BLACK);

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

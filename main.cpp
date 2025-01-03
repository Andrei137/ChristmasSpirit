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
#include "path.h"
#include "shaders.h"

using namespace Utils;

/* Variables Section */
double
    time;
Path
    path;
glm::mat4
    projection;
std::stack<glm::mat4>
    viewStack;

/* Initialization Section */
void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    Shaders::CreateShaders();
    Cone::CreateVBO();
    Cylinder::CreateVBO();
    Sphere::CreateVBO();

    time = 0;
    path = Path::readFromFile("resources/paths/demo_0.dat");
}

void SetMVP()
{
	glm::vec3 obs{ path.interpolate(time) };
	time += 0.01;

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
    Shaders::DestoryShaders();
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

    Shaders::LoadShader("default");
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

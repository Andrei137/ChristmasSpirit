#include <stack>
#include <vector>
#include <iostream>
#include <functional>
#include <filesystem>
#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "bezier/path.h"
#include "helpers/constants.h"
#include "helpers/shaders.h"
#include "helpers/textures.h"
#include "helpers/utils.h"
#include "primitives/circle.h"
#include "primitives/cone.h"
#include "primitives/cylinder.h"
#include "primitives/sphere.h"
#include "mesh.h"

namespace fs = std::filesystem;
using namespace Utils;

/* Variables Section */
std::stack<glm::mat4>
    viewStack;
std::unordered_map<std::string, Mesh>
    meshMap;
std::unordered_map<std::string, Path>
    pathMap;

/* Initialization Section */
void LoadResources()
{
    try
    {
        for (const auto& mesh : fs::directory_iterator(MESHES_PATH))
        {
            assert(mesh.is_regular_file());
            std::string name{ DropFileExtension(mesh.path().filename().string()) };
            meshMap[name].loadMesh(FILE_PATH("mesh", name));
        }
        for (const auto& path : fs::directory_iterator(BEZIER_PATH))
        {
            assert(path.is_regular_file());
            std::string name{ DropFileExtension(path.path().filename().string()) };
            pathMap[name] = Path::readFromFile(FILE_PATH("path", name));
        }
        Shaders::Create();
        Textures::Create();
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    LoadResources();
    Circle::CreateVBO();
    Cone::CreateVBO();
    Cylinder::CreateVBO();
    Sphere::CreateVBO();
}

void SetMVP(const std::string& a_path_name)
{
    static double pathTime{ 0.0f };
    while (!viewStack.empty())
    {
        viewStack.pop();
    }
    viewStack.push(glm::lookAt(pathMap[a_path_name].interpolate(pathTime), REF, VERT));
    pathTime += pathTime < 8.9 ? 0.01 : -pathTime;

    Shaders::SetMVP(
        viewStack.top(),
        glm::infinitePerspective(FOV, GLfloat(width) / GLfloat(height), ZNEAR)
    );
}

/* Cleanup Section */
void Cleanup()
{
    Shaders::Destroy();
    Textures::Destroy();
    Circle::DestroyVBO();
    Cone::DestroyVBO();
    Cylinder::DestroyVBO();
    Sphere::DestroyVBO();
}

/* Demo Section */
void DemoPrimitives()
{
    SetMVP("demo_0");
    int
        timeElapsed{ static_cast<int>(glutGet(GLUT_ELAPSED_TIME)) },
        shaderInterval{ 750 },
        shapeInterval{ 3000 },
        shaderType{ (timeElapsed / shaderInterval) % 2 },
        shapeIdx{ (timeElapsed / shapeInterval) % 3 };

    if (shaderType == 0)
    {
        Shaders::SetDefault();
    }
    else
    {
        Shaders::SetBlack();
    }

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
}

void DemoSnow()
{
    SetMVP("demo_0");
    glEnable(GL_BLEND);
    Circle::Draw();
    glDisable(GL_BLEND);
}

void DemoMesh()
{
    SetMVP("demo_1");
    Shaders::SetMeshDefault("doughnut");
    meshMap["flashlight"].draw();
}

/* Main Section */
void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    std::vector<std::function<void()>> demos = { DemoPrimitives, DemoMesh, DemoSnow };
    demos[Utils::demoIdx]();

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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

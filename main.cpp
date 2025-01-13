#include <stack>
#include <vector>
#include <future>
#include <cstdlib>
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
#include "scene\mesh.h"
#include "scene\scene.h"

namespace fs = std::filesystem;
using namespace Utils;

/* Variables Section */
std::stack<glm::mat4>
    viewStack;
std::unordered_map<std::string, Mesh>
    meshMap;
std::unordered_map<std::string, Path>
    pathMap;
Scene
    scene;

/* Initialization Section */
void LoadResources()
{
    std::vector<std::future<void>> futures;
    std::mutex pathMapMutex;
    std::mutex meshMapMutex;
    try
    {
        for (const auto& path : fs::directory_iterator(BEZIER_PATH))
        {
            assert(path.is_regular_file());
            std::string name{ DropFileExtension(path.path().filename().string()) };
            futures.push_back(std::async(std::launch::async, [name, &pathMapMutex] {
                Path data{ Path::readFromFile(FILE_PATH("path", name)) };
                std::lock_guard<std::mutex> lock(pathMapMutex);
                pathMap[name] = std::move(data);
            }));
        }

        for (const auto& mesh : fs::directory_iterator(MESHES_PATH)) {
            assert(mesh.is_regular_file());
            std::string name{ DropFileExtension(mesh.path().filename().string()) };
            futures.push_back(std::async(std::launch::async, [name, &meshMapMutex] {
                Mesh meshData;
                meshData.loadMesh(FILE_PATH("mesh", name));
                std::lock_guard<std::mutex> lock(meshMapMutex);
                meshMap[name] = std::move(meshData);
            }));
        }
        for (auto& future : futures)
        {
            future.get();
        }

        for (auto& [name, mesh] : meshMap)
        {
            mesh.createGLids();
        }
        Shaders::Create();
        Textures::Create();
		scene.loadScene("scene.txt");
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Initialize()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    LoadResources();
    Circle::CreateVBO(pathMap["snow"].length());
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
    pathTime += pathTime < 15 ? 0.01 : -pathTime;

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

void DemoMesh()
{
    static float time{ 0.f };
    Utils::cameraPos = pathMap["camera"].interpolate(time);
    Utils::cameraOrientation = glm::normalize(pathMap["camera_orient"].interpolate(time));
    scene.draw(meshMap);
    time += 0.01f;
}

void DemoSnow()
{
    SetMVP("demo_0");
    Circle::Draw();
    Circle::UpdateTranslations(pathMap["snow"], 0.01f);
}

/* Main Section */
void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    std::vector<std::function<void()>> demos = { DemoSnow, DemoPrimitives, DemoMesh };
    demos[Utils::demoIdx]();

    glutSwapBuffers();
    glFlush();
}

int main(int argc, char* argv[])
{
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(static_cast<int>(winWidth), static_cast<int>(winHeight));
    glutInitWindowPosition(POSX, POSY);
    glutCreateWindow(TITLE.c_str());
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
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

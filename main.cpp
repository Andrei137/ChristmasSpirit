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
#include "scene/mesh.h"
#include "scene/scene.h"
#include "scene/snow.h"

namespace fs = std::filesystem;
using namespace Utils;

/* Variables Section */
float
    sceneTime{ 0.0f };
std::unordered_map<std::string, Mesh>
    meshMap;
std::unordered_map<std::string, Path>
    pathMap;
Scene
    scene;

/* Initialization Section */
void Initialize()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
        Shaders::SetProj(
            glm::infinitePerspective(FOV, GLfloat(Utils::width) / GLfloat(Utils::height), ZNEAR)
        );
        Textures::Create();
        Snow::CreateVBO(pathMap["snow"], FILE_PATH("scene", "snow"));
        scene.loadScene(FILE_PATH("scene", "objects"));
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << e.what() << '\n';
    }
}

/* Cleanup Section */
void Cleanup()
{
    Shaders::Destroy();
    Textures::Destroy();
    Snow::DestroyVBO();
}

/* Main Section */
void DrawScene()
{
    Utils::cameraPos = pathMap["camera"].interpolate(sceneTime);
    Utils::cameraOrientation = glm::normalize(pathMap["camera_orient"].interpolate(sceneTime));
    Snow::Draw();
    Snow::UpdateTranslations();
    scene.draw(meshMap);
    sceneTime += 0.01f;
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    std::vector<std::function<void()>> scenes = { DrawScene };
    scenes[Utils::demoIdx]();

    glutSwapBuffers();
    glFlush();
}

int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(time(0)));
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

#include "helpers/utils.h"
#include "helpers/constants.h"
#include "SOIL.h"
#include <map>
#include <stdlib.h>
#include <GL/freeglut_std.h>
#include "glm/gtc/matrix_transform.hpp"

namespace Utils
{
    GLfloat
        winWidth{ 1280.0f },
        winHeight{ 720.0f };

    float
        sceneTime{ PATH_TIME_START },
        width{ 800.0f },
        height{ 600.0f };

	glm::vec3
		cameraPos{ glm::vec3(0.f, 0.f, 5.f) },
		cameraOrientation{ glm::vec3(1.f, 0.f, 0.f) },
		cameraVertical{ glm::vec3(0.f, 0.f, -1.f) };

    int
        sceneIdx{ 0 };

    std::map<unsigned char, bool>
        keyState;

    void LoadTexture(const char* a_photoPath, GLuint& a_texture)
    {
        glGenTextures(1, &a_texture);
        glBindTexture(GL_TEXTURE_2D, a_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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

    std::string DropFileExtension(const std::string& a_file)
    {
        return a_file.substr(0, a_file.find_last_of('.'));
    }

    std::string FilePath(const std::string& a_type, const std::string& a_name)
    {
        std::string folder{}, extension{};
        if (a_type == "mesh")
        {
            folder = MESHES_PATH;
            extension = ".obj";
        }
        else if (a_type == "path")
        {
            folder = BEZIER_PATH;
            extension = ".dat";
        }
        else if (a_type == "scene")
        {
            folder = SCENES_PATH,
            extension = ".txt";
        }
        else if (a_type == "shader")
        {
            folder = SHADERS_PATH;
            extension = "/shader";
        }
        else if (a_type == "texture")
        {
            folder = TEXTURES_PATH;
            extension = ".png";
        }
        return folder + "/" + a_name + extension;
    }

    void UpdateCamera(glm::vec3 a_cameraPos, glm::vec3 a_cameraOrientation)
    {
        if (sceneIdx == 2) // free roam
        {
            if (keyState['w'] || keyState['W'])
            {
                cameraPos += cameraOrientation * 0.08f;
            }
            if (keyState['s'] || keyState['S'])
            {
                cameraPos -= cameraOrientation * 0.08f;
            }
            if (keyState['d'] || keyState['D'])
            {
                glm::vec4 aux = glm::rotate(glm::mat4(1.f), 0.01f, cameraVertical) * glm::vec4(cameraOrientation, 1);
                cameraOrientation = glm::vec3(aux.x, aux.y, aux.z);
            }
            if (keyState['a'] || keyState['A'])
            {
                glm::vec4 aux = glm::rotate(glm::mat4(1.f), -0.01f, cameraVertical) * glm::vec4(cameraOrientation, 1);
                cameraOrientation = glm::vec3(aux.x, aux.y, aux.z);
            }
            if (keyState['q'] || keyState['Q'])
            {
                auto rot = glm::rotate(glm::mat4(1.f), -0.01f, glm::cross(cameraVertical, cameraOrientation));
                glm::vec4 aux = rot * glm::vec4(cameraOrientation, 1);
                cameraOrientation = glm::vec3(aux.x, aux.y, aux.z);
                aux = rot * glm::vec4(cameraVertical, 1);
                cameraVertical = glm::vec3(aux.x, aux.y, aux.z);
            }
            if (keyState['e'] || keyState['E'])
            {
                auto rot = glm::rotate(glm::mat4(1.f), 0.01f, glm::cross(cameraVertical, cameraOrientation));
                glm::vec4 aux = rot * glm::vec4(cameraOrientation, 1);
                cameraOrientation = glm::vec3(aux.x, aux.y, aux.z);
                aux = rot * glm::vec4(cameraVertical, 1);
                cameraVertical = glm::vec3(aux.x, aux.y, aux.z);
            }
        }
        else
        {
            cameraPos = a_cameraPos;
            cameraOrientation = a_cameraOrientation;
        }
    }

    void UpdatePathTime(const std::string& a_mode)
    {
        if (sceneIdx == 0 && a_mode == "auto")
        {
            sceneTime = std::min(sceneTime + PATH_TIME_INCREMENT * PATH_SPEED_AUTO, PATH_TIME_END);
        }
        else if (sceneIdx == 1)
        {
            float increment{ PATH_TIME_INCREMENT * PATH_SPEED_MANUAL };
            if (a_mode == "manual_increment")
            {
                sceneTime = std::min(sceneTime + increment, PATH_TIME_END);
            }
            else if (a_mode == "manual_decrement")
            {
                sceneTime = std::max(sceneTime - increment, PATH_TIME_START);
            }
        }
    }

    void ProcessNormalKeys(unsigned char a_key, int, int)
    {
        if (a_key >= '1' && a_key <= '3')
		{
            sceneIdx = (a_key - '0') - 1;
        }
        if (a_key == '+' || a_key == '=')
        {
            UpdatePathTime("manual_increment");
        }
        if (a_key == '-')
        {
            UpdatePathTime("manual_decrement");
        }
        if (a_key == 'r')
        {
            sceneTime = PATH_TIME_START;
        }
        if (sceneIdx == 2)
        {
            keyState[a_key] = true;
        }
        if (a_key == 27)
        {
            exit(0);
        }
    }

    void ProcessKeyUp(unsigned char a_key, int, int)
    {
        keyState[a_key] = false;
    }

    void ProcessSpecialKeys(int a_key, int, int)
    {
        if (a_key == GLUT_KEY_UP)
        {
            keyState['w'] = true;
        }
        if (a_key == GLUT_KEY_DOWN)
        {
            keyState['s'] = true;
        }
        if (a_key == GLUT_KEY_RIGHT)
        {
            keyState['d'] = true;
        }
        if (a_key == GLUT_KEY_LEFT)
        {
            keyState['a'] = true;
        }
    }

    void ProcessSpecialKeyUp(int a_key, int, int)
    {
        if (a_key == GLUT_KEY_UP)
        {
            keyState['w'] = false;
        }
        if (a_key == GLUT_KEY_DOWN)
        {
            keyState['s'] = false;
        }
        if (a_key == GLUT_KEY_RIGHT)
        {
            keyState['d'] = false;
        }
        if (a_key == GLUT_KEY_LEFT)
        {
            keyState['a'] = false;
        }
    }

    void ReshapeWindow(GLint a_newWidth, GLint a_newHeight)
    {
        glViewport(0, 0, a_newWidth, a_newHeight);

        winWidth = static_cast<GLfloat>(a_newWidth);
        winHeight = static_cast<GLfloat>(a_newHeight);
        width = winWidth / 10;
        height = winHeight / 10;
    }
}
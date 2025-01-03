#include "helpers/shaders.h"
#include "helpers/constants.h"
#include "loadShaders.h"
#include <filesystem>
#include <iostream>
#include <GL/freeglut.h>
#include <cassert>

namespace fs = std::filesystem;

namespace Shaders
{
    std::unordered_map<std::string, GLuint>
        shaders{};

    glm::mat4
        view{},
        proj{};

    void Create()
    {
        try
        {
            for (const auto& shader_dir : fs::directory_iterator(SHADERS_PATH))
            {
                assert(shader_dir.is_directory());
                std::string name{ shader_dir.path().filename().string() };
                std::string path{ SHADERS_PATH + "/" + name + "/shader" };
                shaders[name] = LoadShaders(
                    (path + ".vert").c_str(),
                    (path + ".frag").c_str()
                );
            }
        }
        catch (const fs::filesystem_error& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void SetMVP(glm::mat4 a_view, glm::mat4 a_proj)
    {
        view = a_view;
        proj = a_proj;
    }

    void LoadShader(std::string a_name)
    {
        GLuint shaderID{ shaders[a_name] };
        GLint viewLocation{ glGetUniformLocation(shaderID, "viewShader") };
        GLint projLocation{ glGetUniformLocation(shaderID, "projectionShader") };

        glUseProgram(shaderID);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, &proj[0][0]);
    }

    void Destroy()
    {
        for (const auto& shader : shaders)
        {
            glDeleteProgram(shader.second);
        }
    }

    // API to change the current shader
    void SetDefault()
    {
        LoadShader("default");
    }

    void SetBlack()
    {
        LoadShader("black");
    }
}
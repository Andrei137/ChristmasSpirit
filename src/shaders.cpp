#include "shaders.h"
#include "constants.h"
#include "loadShaders.h"
#include <filesystem>
#include <iostream>
#include <GL/freeglut.h>

namespace fs = std::filesystem;

namespace Shaders
{
    std::unordered_map<std::string, GLuint>
        shaders{};

    glm::mat4
        view{},
        projection{};

    void CreateShaders()
    {
        try
        {
            for (const auto& shader_dir : fs::directory_iterator(SHADERS_PATH))
            {
                if (shader_dir.is_directory())
                {
                    std::string name{ shader_dir.path().filename().string() };
                    std::string path{ SHADERS_PATH + "/" + name + "/shader" };
                    shaders[shader_dir.path().filename().string()] = LoadShaders(
                        (path + ".vert").c_str(),
                        (path + ".frag").c_str()
                    );
                }
            }
        }
        catch (const fs::filesystem_error& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void SetMVP(glm::mat4 view, glm::mat4 projection)
    {
        Shaders::view = view;
        Shaders::projection = projection;
    }

    void LoadShader(std::string name)
    {
        GLint viewLocation{ glGetUniformLocation(shaders[name], "viewShader") };
        GLint projLocation{ glGetUniformLocation(shaders[name], "projectionShader") };

        glUseProgram(shaders[name]);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
    }

    void DestoryShaders()
    {
        for (const auto& shader : shaders)
        {
            glDeleteProgram(shader.second);
        }
    }

}
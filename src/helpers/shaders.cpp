#include "helpers/shaders.h"
#include "helpers/textures.h"
#include "helpers/constants.h"
#include "helpers/utils.h"
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
        proj{};

    void Create()
    {
        try
        {
            for (const auto& shader_dir : fs::directory_iterator(SHADERS_PATH))
            {
                assert(shader_dir.is_directory());
                std::string name{ shader_dir.path().filename().string() };
                std::string path{ Utils::FILE_PATH("shader", name) };
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

    void Destroy()
    {
        for (const auto& shader : shaders)
        {
            glDeleteProgram(shader.second);
        }
    }

    void SetView(glm::mat4 a_view)
    {
        view = a_view;
    }

    void SetProj(glm::mat4 a_proj)
    {
        proj = a_proj;
    }

    void SetShader(std::string a_name)
    {
        GLuint shaderID{ shaders[a_name] };
        glUseProgram(shaderID);
        Utils::SetUniformMat(shaderID, "projectionShader", proj);
        Utils::SetUniformMat(shaderID, "viewShader", view);
    }

    // API to change the current shader
    void SetDefault()
    {
        SetShader("default");
    }

    void SetBlack()
    {
        SetShader("black");
    }

    void SetMeshDefault(std::string a_texture_name, glm::mat4 a_tranformation)
    {
    	SetShader("mesh_default");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Textures::Get(a_texture_name));
        Utils::SetUniformMat(shaders["mesh_default"], "model2world", a_tranformation);
        Utils::SetUniformInt(shaders["mesh_default"], "textureShader", 0);
    }

    void SetSnow(glm::mat4 a_translation)
    {
        SetShader("snow");
        Utils::SetUniformFloat(shaders["snow"], "time", glutGet(GLUT_ELAPSED_TIME) * DELTA_TIME_SNOW);
        Utils::SetUniformMat(shaders["snow"], "translateToLocation", a_translation);
    }
}

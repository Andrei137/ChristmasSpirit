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

    void SetMVP(glm::mat4 a_view, glm::mat4 a_proj)
    {
        if (a_view != glm::mat4(0))
        {
            view = a_view;
        }
        if (a_proj != glm::mat4(0))
        {
            proj = a_proj;
        }
    }

    void SetShader(std::string a_name, glm::mat4 a_custom_view = glm::mat4(0))
    {
        GLuint shaderID{ shaders[a_name] };
        glm::mat4 viewToUse{ a_custom_view != glm::mat4(0) ? a_custom_view : view };

        glUseProgram(shaderID);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "projectionShader"), 1, GL_FALSE, &proj[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "viewShader"), 1, GL_FALSE, &viewToUse[0][0]);
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
        glUniformMatrix4fv(glGetUniformLocation(shaders["mesh_default"], "model2world"), 1, GL_FALSE, &a_tranformation[0][0]);
        glUniform1i(glGetUniformLocation(shaders["mesh_default"], "textureShader"), 0);
    }

    void SetCircle(glm::mat4 a_view)
    {
        SetShader("circle", a_view);
    }
}

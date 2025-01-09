#include "helpers/textures.h"
#include "helpers/constants.h"
#include "helpers/utils.h"
#include <filesystem>
#include <iostream>
#include <GL/freeglut.h>

namespace fs = std::filesystem;
using namespace Utils;

namespace Textures
{
    std::unordered_map<std::string, GLuint>
        textures{};

    void Create()
    {
        try
        {
            for (const auto& texture : fs::directory_iterator(TEXTURES_PATH))
            {
                assert(texture.is_regular_file());
                std::string name{ DropFileExtension(texture.path().filename().string()) };
                LoadTexture(FILE_PATH("texture", name), textures[name]);
            }
        }
        catch (const fs::filesystem_error& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void Destroy()
    {
        for (const auto& texture : textures)
        {
            glDeleteTextures(1, &texture.second);
        }
    }

    GLuint Get(std::string a_name)
    {
        return textures[a_name];
    }
}
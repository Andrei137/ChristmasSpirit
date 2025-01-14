#include "helpers/utils.h"
#include "helpers/constants.h"
#include "SOIL.h"
#include <stdlib.h>
#include <GL/freeglut_std.h>

namespace Utils
{
    GLfloat
        winWidth{ 1280.0f },
        winHeight{ 720.0f };

    float
        width{ 800.0f },
        height{ 600.0f };

	glm::vec3
		cameraPos{ glm::vec3(0.f, 0.f, 5.f) },
		cameraOrientation{ glm::vec3(1.f, 0.f, 0.f) },
		cameraVertical{ glm::vec3(0.f, 0.f, -1.f) };

    int
        demoIdx{ 0 };

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

    void SetUniformInt(GLuint a_ID, const char* a_name, int a_value)
    {
        glUniform1i(
            glGetUniformLocation(a_ID, a_name),
            a_value
        );
    }

    void SetUniformFloat(GLuint a_ID, const char* a_name, float a_value)
    {
        glUniform1f(
            glGetUniformLocation(a_ID, a_name),
            a_value
        );
    }

    void SetUniformMat(GLuint a_ID, const char* a_name, glm::mat4 a_mat)
    {
        glUniformMatrix4fv(
            glGetUniformLocation(a_ID, a_name),
            1, GL_FALSE, &a_mat[0][0]
        );
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

    void ProcessNormalKeys(unsigned char a_key, int, int)
    {
        if (a_key >= '1' && a_key <= '1')
		{
            demoIdx = (a_key - '0') - 1;
        }
        if (a_key == 27)
        {
            exit(0);
        }
    }

    void ProcessSpecialKeys(int a_key, int, int)
    {
        switch (a_key)
        {
            case GLUT_KEY_LEFT:
                --demoIdx;
                if (demoIdx < 0) demoIdx = 0;
                break;
            case GLUT_KEY_RIGHT:
                ++demoIdx;
                if (demoIdx > 0) demoIdx = 0;
                break;
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
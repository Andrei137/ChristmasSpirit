#include "utils.h"
#include "Constants.h"
#include "SOIL.h"
#include <stdlib.h>
#include <GL/freeglut_std.h>

namespace Utils
{
    GLfloat
        winWidth{ 1280.f },
        winHeight{ 720.f };

    float
        dist{ 300.0f },
        alpha{ 0.0f },
        beta{ 0.0f },
        width{ 800.f },
        height{ 600.f };

    void LoadTexture(const char* a_photoPath, GLuint& a_texture)
    {
        glGenTextures(1, &a_texture);
        glBindTexture(GL_TEXTURE_2D, a_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
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

    void ProcessNormalKeys(unsigned char key, int, int)
    {
        switch (key) {
            case '-':
                dist -= 5.0;
                break;
            case '+':
                dist += 5.0;
                break;
        }
        if (key == 27)
        {
            exit(0);
        }
    }

    void ProcessSpecialKeys(int key, int, int)
    {
        switch (key)
        {
            case GLUT_KEY_LEFT:
                beta -= 0.01f;
                break;
            case GLUT_KEY_RIGHT:
                beta += 0.01f;
                break;
            case GLUT_KEY_UP:
                if (abs(alpha - PI / 2) >= 0.05)
                {
                    alpha += 0.01f;
                }
                break;
            case GLUT_KEY_DOWN:
                if (abs(alpha + PI / 2) >= 0.05)
                {
                    alpha -= 0.01f;
                }
                break;
        }
    }

    void ReshapeWindow(GLint newWidth, GLint newHeight)
    {
        glViewport(0, 0, newWidth, newHeight);

        winWidth = newWidth;
        winHeight = newHeight;
        width = winWidth / 10;
        height = winHeight / 10;
    }
}
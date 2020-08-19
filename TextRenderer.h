#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <chrono>
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H


namespace JuicyEngineNS
{


    class TextRenderer
    {
    public:
        TextRenderer(const char* fontPath, const char* vertShader, const char* fragShader, int fSize, int winWidth, int winHeight);
        ~TextRenderer();

        struct CharacterFont {

            unsigned int TexID;      // ID handle of the glyph texture
            glm::ivec2   Size;       // Size of glyph
            glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
            unsigned int Advance;    // Offset to advance to next glyph
            int charWidth;
        };

        struct InputCursor {
            float xPos;
            float yPos;
            bool active;



        };
        InputCursor textCursor;
        unsigned int FONT_ID;
        unsigned int VAO;
        unsigned int VBO;
        int fontSize;
        Shader* fontShader;

        //--------------framebuffer-------------

        //--------------------------------------

        FT_Library ft_library;
        FT_Face fontFace;
        CharacterFont CharacterArray[256];

        int FONT_MAX_HEIGHT;
        int FONT_MAX_WIDTH;

        void DrawString(std::string text, float x, float y, float scale, glm::vec3 color);
        void DrawCharacter(char c, float x, float y, float scale, glm::vec3 color);
        void DrawCursor(float scale, glm::vec3 color);

    };

}
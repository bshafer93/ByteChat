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

        void DrawString(std::string text, float x, float y, float scale, glm::vec3 color);
        void DrawCharacter(char c, float x, float y, float scale, glm::vec3 color);
        void DrawCursor(float scale, glm::vec3 color);


        InputCursor text_cursor;
        unsigned int font_id;
        unsigned int vao;
        unsigned int vbo;
        int font_size;
        Shader* font_shader;

        FT_Library ft_library;
        FT_Face font_face;
        CharacterFont character_array[256];

        int font_max_height;
        int font_max_width;

    };

}
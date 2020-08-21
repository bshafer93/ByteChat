#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <chrono>
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H


namespace BByteEngineNS
{

    class TextRenderer
    {
    public:
        TextRenderer(const char* fontPath, const char* vertShader, const char* fragShader, int fSize, int winWidth, int winHeight);
        ~TextRenderer();

        struct CharacterFont {

            unsigned int texture_id;      // ID handle of the glyph texture
            glm::ivec2   size;       // Size of glyph
            glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
            unsigned int offset_to_next;    // Offset to advance to next glyph
            int character_width;
        };
        struct InputCursor {
            float x_position;
            float y_position;
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
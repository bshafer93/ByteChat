#include "TextRenderer.h"
namespace JuicyEngineNS
{

	TextRenderer::TextRenderer(const char* fPath, const char* vertShader, const char* fragShader, int fSize, int winWidth, int winHeight)
	{
		textCursor.active = false;
		fontSize = fSize;
		std::string vertS, fragS, fontPath;
		vertS.append(vertShader);
		fragS.append(fragShader);
		fontPath.append(fPath);
		fontShader = new Shader(vertS.c_str(), fragS.c_str());




		if (FT_Init_FreeType(&ft_library)) {
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}

		if (fPath == NULL) {
			if (FT_New_Face(ft_library, "I:\\Programming\\SpaceAdventure\\resources\\font\\AtariClassicChunky.ttf", 0, &fontFace)) {
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			}
		}
		else if (FT_New_Face(ft_library, fontPath.c_str(), 0, &fontFace)) {
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}

		std::cout << "Number Of Glyphs In Font: " << fontFace->num_glyphs << std::endl;

		//setFontsize
		FT_Set_Pixel_Sizes(fontFace, 0, fontSize);



		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			FT_UInt glyph_index; //Unicode index 

			glyph_index = FT_Get_Char_Index(fontFace, c); // Get unicode index of Char;

			/* load glyph image into the slot (erase previous one) */
			if (FT_Load_Glyph(fontFace, glyph_index, FT_LOAD_RENDER)) {
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			if (c == 'H') {
				FONT_MAX_HEIGHT = fontFace->glyph->bitmap.rows;
			}
			if (c == 'W') {
				FONT_MAX_WIDTH = fontFace->glyph->bitmap.width;
			
			}


			// generate texture
			unsigned int textureFont_ID;
			glGenTextures(1, &textureFont_ID);

			glBindTexture(GL_TEXTURE_2D, textureFont_ID);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				fontFace->glyph->bitmap.width,
				fontFace->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				fontFace->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			CharacterFont character = {
				textureFont_ID,
				glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
				glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
				fontFace->glyph->advance.x,
				fontFace->glyph->bitmap.width
			};

			CharacterArray[c] = character;
		}



		FT_Done_Face(fontFace);
		FT_Done_FreeType(ft_library);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(winWidth), static_cast<float>(winHeight), 0.0f);
		fontShader->activate();
		fontShader->setUniformMat4("projection", proj);



	}

	TextRenderer::~TextRenderer()
	{
		for (int i = 0; i < 256; i++) {
			glDeleteTextures(1, &CharacterArray[i].TexID);
		}
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		delete fontShader;
		

	}

	void TextRenderer::DrawString(std::string text, float x, float y, float scale, glm::vec3 color)
	{
		textCursor.xPos = x;
		textCursor.yPos = y;

		// activate corresponding render state	
		fontShader->activate();
		glUniform3f(glGetUniformLocation(fontShader->ID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{

			CharacterFont ch = CharacterArray[*c];

			float xpos = textCursor.xPos + ch.Bearing.x * scale;
			float ypos = textCursor.yPos + (CharacterArray['H'].Bearing.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float font_vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 0.0f },

				{ xpos,     ypos + h,   0.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TexID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(font_vertices), font_vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			textCursor.xPos += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);


	}

	void TextRenderer::DrawCharacter(char c, float x, float y, float scale, glm::vec3 color)
	{

		std::string s;
		s += c;
		DrawString(s, x, y, scale, color);

	}

	void TextRenderer::DrawCursor(float scale, glm::vec3 color)
	{
		static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();


		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - oldTime) > std::chrono::milliseconds{ 560 }) {
			oldTime = std::chrono::high_resolution_clock::now();
			textCursor.active = !textCursor.active;
		}

		if (textCursor.active == true) {
			// activate corresponding render state	
			fontShader->activate();
			glUniform3f(glGetUniformLocation(fontShader->ID, "textColor"), color.x, color.y, color.z);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);


			CharacterFont ch = CharacterArray[124];

			float xpos = textCursor.xPos + ch.Bearing.x * scale;
			float ypos = textCursor.yPos + (CharacterArray['H'].Bearing.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float font_vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 0.0f },

				{ xpos,     ypos + h,   0.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TexID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(font_vertices), font_vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);



			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);

		}


	}


}
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace BByteEngineNS
{

	Texture::Texture(const char* texPath, bool alpha)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		if (!alpha)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}

		std::string num;
		num.append(texPath);

		data = stbi_load(texPath, &width, &height, &number_of_channels, 0);
		std::cout << "Color Channels: " << number_of_channels << std::endl;

		if (data) {
			if (number_of_channels == 4) {

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				std::cout << "Texture Loaded with Alpha" << std::endl;
			}
			else {

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				std::cout << "Texture Loaded without Alpha" << std::endl;
			}
		}
		else {
			std::cout << "Texture Couldnt Load" << std::endl;
		}

		stbi_image_free(data);

	}

	Texture::~Texture()
	{
	}

	void Texture::activate(int n)
	{
		switch (n) {
		case 0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
			glActiveTexture(GL_TEXTURE1);
			break;
		default:
			glActiveTexture(GL_TEXTURE0);
		}

		glBindTexture(GL_TEXTURE_2D, id);

	}
}
#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace BByteEngineNS
{


	class Texture
	{
	public:
		Texture(const char* texPath, bool apha);
		~Texture();

		void activate(int n);

		unsigned char* data;
		unsigned int id = 0;
		int width;
		int height;
		int number_of_channels;
	};

}
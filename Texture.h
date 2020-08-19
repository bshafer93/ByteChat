#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace JuicyEngineNS
{


	class Texture
	{
	public:
		Texture(const char* texPath, bool apha);
		~Texture();

		void activate(int n);

		unsigned char* data;
		unsigned int ID = 0;
		int width;
		int height;
		int numChannels;
	};

}
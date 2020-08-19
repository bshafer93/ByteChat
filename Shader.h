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

	class Shader
	{
	public:
		Shader(const char* vertPath, const char* fragPath);
		~Shader();
		unsigned int ID;
		void activate();

		void setUniformBool(std::string& uniformName, bool val);
		void setUniformInt(std::string& uniformName, int val);
		void setUniformFloat(std::string& uniformName, float val);
		void setUniformMat4(std::string& uniformName, glm::mat4 tran);
		void setUniformMat4(const char* uniformName, glm::mat4 tran);

		std::string loadFile(const char* path);
	};

}
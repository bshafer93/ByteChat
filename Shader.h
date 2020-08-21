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
		void Activate();

		void SetUniformBool(std::string& uniformName, bool val);
		void SetUniformInt(std::string& uniformName, int val);
		void SetUniformFloat(std::string& uniformName, float val);
		void SetUniformMat4(std::string& uniformName, glm::mat4 tran);
		void SetUniformMat4(const char* uniformName, glm::mat4 tran);

		std::string LoadFile(const char* path);
	};

}
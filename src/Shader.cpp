#include "Shader.h"
namespace JuicyEngineNS
{

	Shader::Shader(const char* vertP, const char* fragP)
	{

		std::string vertContents, fragContents;

		vertContents = LoadFile(vertP);
		fragContents = LoadFile(fragP);
		const char* vert = vertContents.c_str();
		const char* frag = fragContents.c_str();
		unsigned int vert_id = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vert_id, 1, &vert, NULL);//(shader object id,number of strings in source code,the actual code,???)
		glCompileShader(vert_id);

		//Error checking
		int success;
		char log[512];
		glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vert_id, 512, NULL, log);
			std::cout << "VERTEXSHADER::COMPILATION_FAILED\n" << log << std::endl;
		}



		unsigned int frag_id = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(frag_id, 1, &frag, NULL);//(shader object id,number of strings in source code,the actual code,???)
		glCompileShader(frag_id);
		glGetShaderiv(frag_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(frag_id, 512, NULL, log);
			std::cout << "FRAGMENTSHADER::COMPILATION_FAILED\n" << log << std::endl;
		}



		id = glCreateProgram();
		glAttachShader(id, vert_id);
		glAttachShader(id, frag_id);
		glLinkProgram(id);

		// check for linking errors
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 512, NULL, log);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
		}

		//Clear shaders after loading
		glDeleteShader(vert_id);
		glDeleteShader(frag_id);


	}

	Shader::~Shader()
	{
		glDeleteProgram(id);
	}

	void Shader::Activate()
	{
		glUseProgram(id);
	}

	void Shader::SetUniformMat4(std::string& uniformName, glm::mat4 tran)
	{
		int location = glGetUniformLocation(id, uniformName.c_str());

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(tran));
	}

	void Shader::SetUniformMat4(const char* uniformName, glm::mat4 tran)
	{

		int location = glGetUniformLocation(id, uniformName);

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(tran));
	}

	void Shader::SetUniformBool(std::string& uniformName, bool val)
	{
		int location = glGetUniformLocation(id, uniformName.c_str());

		glUniform1i(location, (int)val);


	}

	void Shader::SetUniformInt(std::string& uniformName, int val)
	{
		int location = glGetUniformLocation(id, uniformName.c_str());

		glUniform1i(location, (int)val);


	}

	void Shader::SetUniformFloat(std::string& uniformName, float val)
	{
		int location = glGetUniformLocation(id, uniformName.c_str());

		glUniform1i(location, (int)val);


	}

	std::string Shader::LoadFile(const char* path)
	{

		std::fstream loadedFile;
		loadedFile.open(path, std::ios::in);

		std::string line = "";
		std::string full_file_string;

		while (!loadedFile.eof()) {
			std::getline(loadedFile, line);
			full_file_string.append(line + "\n");
		}
		full_file_string.append("\0");

		loadedFile.close();




		return full_file_string;
	}

}
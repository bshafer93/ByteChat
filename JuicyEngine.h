#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <glad/glad.h>//Need to be included before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <queue>

#include <fstream>
#include <math.h>
#include "Shader.h"
#include "Texture.h"
#include "TextRenderer.h"

#include "BasicChatClient.h"
#include <chrono>

const int LINE_SPACING = 10.0f;
const int FONT_SIZE = 16.0f;
#ifndef _WIN32
#define FONTLOCATION "AtariClassicChunky.ttf"
#endif
#ifdef _WIN32
#define FONTLOCATION "C:\\Windows\\Fonts\\arial.ttf"
#endif

#define FONTVERTSHADER "fontVertShader.vert"
#define FONTFRAGSHADER "fontFragShader.frag"
namespace JuicyEngineNS
{

	class Primative;
	class JuicyEngine
	{

	public:
		int window_height = 600;
		int window_width = 870;

		int MAX_ROWS = 80;
		int MAX_COLS = 30;

		JuicyEngine();
		~JuicyEngine();

		//--------------VARIABLES----------------
		GLFWwindow* window;
		Shader* shader1;
		Shader* shader2;
		Shader* fontShader;
		std::vector<Shader> shaderList;

		TextRenderer* text;

		Texture* tex1;
		Texture* tex2;

		glm::vec3 default_font_color;

		unsigned int textureIDS[128];

		std::vector<Primative*> primShapes;

		std::vector<unsigned int> VAOS;
		std::vector<unsigned int> VBOS;

		std::vector<Shader*> ShaderList;
		std::vector<Texture*> TextureList;

		//-----Camera variables-----------------
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		//---------------------------------------

		//-----------Time Related Things----------
		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame
		//---------------------------------------

		//---------------------Keyinput----------
		typedef struct keyInputStruct {
			int key;
			int scancode;
			int action;
			int mods;
		};

		std::queue<keyInputStruct>keyInputQueue;
		std::string inputText;
		bool getInput;
		//---------------------------------------

		enum class primativeType {
			CIRCLE,
			SQUARE,
			TRIANGLE,
			CUBE,
			SPHERE,
			PYRAMID,
			CONE
		};

		int init_glfw_glad();
		static void callback_WindowResize(GLFWwindow* window, int width, int height);
		static void character_callback(GLFWwindow* window, unsigned int codepoint);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void processInput(GLFWwindow* window);
		int renderLoop();
		void run();

		void clearFrameBuffer();
		void addPrimative(Shader* s, primativeType p);
		//Add new Prim with custom Shader
		void addPrimative(primativeType p, std::string vertPath, std::string fragPath);
		void addPrimative(primativeType p, const char* vertPath, const char* fragPath);
		//--------------HWINFO----------
		int nrAttributes;
		//-----------------------------


		//----------------framebuffer---
		void SetUpFrameBuffer();
		unsigned int VAO_SCREEN;
		unsigned int VBO_SCREEN;
		unsigned int FrameBuffer_ID;
		unsigned int textureColorBuffer;
		unsigned int RBO_SCREEN;
		Shader* screenShader;
		//-------------------------------

		//Networking Class---------------
		BasicChatClient client;
		//-------------------------------

	};


	std::string loadFile(const char* path);


}
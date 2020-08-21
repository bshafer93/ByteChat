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
#define FONTLOCATION "Fonts\\AtariClassicChunky.ttf"
#endif
#ifdef _WIN32
#define FONTLOCATION "C:\\Windows\\Fonts\\arial.ttf"
#endif

#define FONTVERTSHADER "Shaders\\fontVertShader.vert"
#define FONTFRAGSHADER "Shaders\\fontFragShader.frag"

namespace JuicyEngineNS
{

	
	class JuicyEngine
	{

	public:
		int window_height = 600;
		int window_width = 870;

		int max_rows = 80;
		int max_columns = 30;

		JuicyEngine();
		~JuicyEngine();

		//--------------VARIABLES----------------
		GLFWwindow* window;
	
		TextRenderer* text;
		glm::vec3 default_font_color;
	
		//-----Camera variables-----------------
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		//---------------------------------------

		//-----------Time Related Things----------
		float delta_time = 0.0f;	// Time between current frame and last frame
		float last_time_frame = 0.0f; // Time of last frame
		//---------------------------------------

		//---------------------Keyinput----------
		typedef struct keyInputStruct {
			int key;
			int scancode;
			int action;
			int mods;
		};

		std::queue<keyInputStruct>key_input_queue;
		std::string input_text;
		bool get_input;
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

		void ProcessInput(GLFWwindow* window);
		int RenderLoop();
		void Run();
		void ClearFrameBuffer();
		
		//--------------HWINFO----------
		int nr_attributes;
		//-----------------------------


		//----------------framebuffer---
		void SetUpFrameBuffer();
		unsigned int vao_screen;
		unsigned int vbo_screen;
		unsigned int frame_buffer_id;
		unsigned int texture_color_buffer;
		unsigned int rbo_screen;
		std::unique_ptr<Shader> screen_shader;
		//-------------------------------

		//Networking Class---------------
		BasicChatClient client;
		//-------------------------------

	};


	std::string LoadFile(const char* path);


}
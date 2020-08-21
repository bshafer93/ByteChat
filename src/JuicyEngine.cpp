#include "JuicyEngine.h"
#include "Shader.h"


namespace JuicyEngineNS
{
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	float quadVertices[] = { 
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	JuicyEngine::JuicyEngine()
	{
		get_input = false;
		init_glfw_glad();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);

		//Initialize font----------------------- 
		text = new TextRenderer(FONTLOCATION, FONTVERTSHADER, FONTFRAGSHADER, FONT_SIZE, window_width, window_height);
		//--------------------------------------

		max_rows = window_height / (text->font_max_height + LINE_SPACING); //10pix padding
		max_columns = window_width / (text->font_max_width / 2);

		std::cout << "MAX_ROWS:	" << max_rows << std::endl;
		std::cout << "MAX_COLS:	" << max_columns << std::endl;

		//Sets up the screenbuffer
		SetUpFrameBuffer();
		//------------------------

		//Set Up Networking---------------------
		client.Initialize();
		//--------------------------------------

		default_font_color = glm::vec3(0.5f, 0.8f, 0.2f);

	}

	JuicyEngine::~JuicyEngine()
	{

		glfwTerminate();
	}

	int JuicyEngine::init_glfw_glad()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		window = glfwCreateWindow(window_width, window_height, "Byte Chat Version 0.0.1", NULL, NULL);


		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		//Transform Pixel cordinates to values between -1 to 1
		glViewport(0, 0, window_width, window_height);

		//Set current context
		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

		
		glfwSetFramebufferSizeCallback(window, callback_WindowResize);
		glfwSetCharCallback(window, character_callback);
		glfwSetKeyCallback(window, key_callback);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr_attributes);

		return 0;

	}

	void JuicyEngine::callback_WindowResize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);

		JuicyEngine* JS;
		JS = reinterpret_cast<JuicyEngine*>(glfwGetWindowUserPointer(window));
		JS->window_height = height;
		JS->window_width = width;
		//remove old font... This wastes SO much resources but oh well
		delete JS->text;
		JS->text = new TextRenderer(FONTLOCATION, FONTVERTSHADER, FONTFRAGSHADER,FONT_SIZE, JS->window_width, JS->window_height);
		JS->SetUpFrameBuffer();
		std::cout << "Window Width: " << JS->window_width << std::endl;
		std::cout << "Window Height: " << JS->window_height << std::endl;
		JS->max_rows = JS->window_height / (JS->text->font_max_height+5); //10pix padding
		JS->max_columns = JS->window_width / (JS->text->font_max_width/2);
		std::cout << "MAX_ROWS:	" << JS->max_rows << std::endl;
		std::cout << "MAX_COLS:	" << JS->max_columns << std::endl;

	}

	void JuicyEngine::character_callback(GLFWwindow* window, unsigned int codepoint)
	{
		JuicyEngine* JS;
		JS = reinterpret_cast<JuicyEngine*>(glfwGetWindowUserPointer(window));
		JS->input_text += (char)codepoint;

	}

	void JuicyEngine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		JuicyEngine* JS;
		JS = reinterpret_cast<JuicyEngine*>(glfwGetWindowUserPointer(window));
		keyInputStruct ki = { key,scancode,action,mods };
		JS->key_input_queue.push(ki);

	}

	void JuicyEngine::ProcessInput(GLFWwindow* window)
	{
		if (key_input_queue.empty()) {
			return;
		}

		if ((key_input_queue.front().key == GLFW_KEY_TAB) && (key_input_queue.front().action == GLFW_PRESS))
		{
			get_input = !get_input;
			std::cout << "Gathering Input" << std::endl;
		}

		if ((key_input_queue.front().key == GLFW_KEY_ESCAPE) && (key_input_queue.front().action == GLFW_PRESS)) {
			glfwSetWindowShouldClose(window, true);
		}

		if (get_input == true)
		{
			if ((key_input_queue.front().key == GLFW_KEY_BACKSPACE) && (key_input_queue.front().action == GLFW_PRESS)) {
				if (!input_text.empty()) {
					input_text.pop_back();
				}	
			}
			if ((key_input_queue.front().key == GLFW_KEY_ENTER) && (key_input_queue.front().action == GLFW_PRESS)) {
				
				get_input = false;
				
				//Send finished input
				std::cout << "Sending Packet" << std::endl;
				client.ParseUserInput(input_text);
			}

		}
		if (get_input == false)
		{
			//Clear input buffer
			input_text.clear();

		}


		key_input_queue.pop();
	}

	int JuicyEngine::RenderLoop()
	{

	

		while (!glfwWindowShouldClose(window))
		{
		
			//----------Get Delta Time---------------
			float currentFrame = glfwGetTime();
			delta_time = currentFrame - last_time_frame;
			last_time_frame = currentFrame;
			//----------------------------------------

			//input
			ProcessInput(window);

			//-----------Clear Current Frame-----------
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
			glEnable(GL_DEPTH_TEST);
			ClearFrameBuffer();
			//-----------------------------------------

			if (!client.message_log.empty()) {
				for (int i = 0; i < max_rows-1; i++) {
					if (i < client.message_log.size()) {


						if (client.message_log[i].size() > max_columns) {
							text->DrawString(client.message_log[i].substr(0,max_columns).c_str(),
								10.0f,
								(i * (text->font_max_height + LINE_SPACING)) + LINE_SPACING,
								1.0f,
								glm::vec3(0.5f, 0.8f, 0.2f));
							i++;
							text->DrawString(client.message_log[i - 1].substr(max_columns, client.message_log[i-1].size()-1).c_str(),
								10.0f,
								(i * (text->font_max_height + LINE_SPACING)) + LINE_SPACING,
								1.0f,
								glm::vec3(0.5f, 0.8f, 0.2f));
						}
						else {
							text->DrawString(client.message_log[i].c_str(),
								10.0f,
								(i * (text->font_max_height + LINE_SPACING)) + LINE_SPACING,
								1.0f,
								glm::vec3(0.5f, 0.8f, 0.2f));
						}


					}
					else {

					}
					
				}
			}


			if (get_input == true) {

				text->DrawString(input_text.c_str(), 10.0f , window_height - text->font_max_height-5.0f,1.0f, default_font_color);
				text->DrawCursor(1.0f, default_font_color);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			ClearFrameBuffer();

			screen_shader->Activate();
			glBindVertexArray(vao_screen);
			glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;





	}

	void JuicyEngine::Run()
	{
		//Run is still here just incase I wanna add anything else before the render loop starts. 

			//Launch seperate thread for recieving packets;
		std::future<void> recmsg = std::async(std::launch::async, &BasicChatClient::Run, &client);
		RenderLoop();

	}

	static std::string LoadFile(const char* path)
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

	void JuicyEngine::ClearFrameBuffer()
	{

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}



	void JuicyEngine::SetUpFrameBuffer()
	{
		if (screen_shader != nullptr) {
			//Remove old buffers when window resizes
			glDeleteFramebuffers(1, &frame_buffer_id);
			glDeleteTextures(1, &texture_color_buffer);
			glDeleteRenderbuffers(1, &rbo_screen);
			glDeleteVertexArrays(1, &vao_screen);
			glDeleteBuffers(1, &vbo_screen);
			
		}

		screen_shader.reset( new Shader("Shaders\\screenShader.vert", "Shaders\\screenShader.frag"));

		// screen quad VAO
		glGenVertexArrays(1, &vao_screen);
		glGenBuffers(1, &vbo_screen);
		glBindVertexArray(vao_screen);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_screen);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


		//framebuffer configuration
		// -------------------------

		glGenFramebuffers(1, &frame_buffer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

		// create a color attachment texture
		glGenTextures(1, &texture_color_buffer);
		glBindTexture(GL_TEXTURE_2D, texture_color_buffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo_screen);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_screen);
		// use a single renderbuffer object for both a depth AND stencil buffer.
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_screen); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//------------------------------------------------------------
	}





}



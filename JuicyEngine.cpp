#include "JuicyEngine.h"
#include "Shader.h"
#include "primative.h"

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
		getInput = false;
		init_glfw_glad();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);

		//Initialize font----------------------- 
		text = new TextRenderer(FONTLOCATION, FONTVERTSHADER, FONTFRAGSHADER, FONT_SIZE, window_width, window_height);
		//--------------------------------------

		MAX_ROWS = window_height / (text->FONT_MAX_HEIGHT + LINE_SPACING); //10pix padding
		MAX_COLS = window_width / (text->FONT_MAX_WIDTH / 2);

		std::cout << "MAX_ROWS:	" << MAX_ROWS << std::endl;
		std::cout << "MAX_COLS:	" << MAX_COLS << std::endl;

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
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

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
		JS->MAX_ROWS = JS->window_height / (JS->text->FONT_MAX_HEIGHT+5); //10pix padding
		JS->MAX_COLS = JS->window_width / (JS->text->FONT_MAX_WIDTH/2);
		std::cout << "MAX_ROWS:	" << JS->MAX_ROWS << std::endl;
		std::cout << "MAX_COLS:	" << JS->MAX_COLS << std::endl;

	}

	void JuicyEngine::character_callback(GLFWwindow* window, unsigned int codepoint)
	{
		JuicyEngine* JS;
		JS = reinterpret_cast<JuicyEngine*>(glfwGetWindowUserPointer(window));
		JS->inputText += (char)codepoint;

	}

	void JuicyEngine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		JuicyEngine* JS;
		JS = reinterpret_cast<JuicyEngine*>(glfwGetWindowUserPointer(window));
		keyInputStruct ki = { key,scancode,action,mods };
		JS->keyInputQueue.push(ki);

	}

	void JuicyEngine::processInput(GLFWwindow* window)
	{
		if (keyInputQueue.empty()) {
			return;
		}

		if ((keyInputQueue.front().key == GLFW_KEY_TAB) && (keyInputQueue.front().action == GLFW_PRESS))
		{
			getInput = !getInput;
			std::cout << "Gathering Input" << std::endl;
		}

		if ((keyInputQueue.front().key == GLFW_KEY_ESCAPE) && (keyInputQueue.front().action == GLFW_PRESS)) {
			glfwSetWindowShouldClose(window, true);
		}

		if (getInput == true)
		{
			if ((keyInputQueue.front().key == GLFW_KEY_BACKSPACE) && (keyInputQueue.front().action == GLFW_PRESS)) {
				if (!inputText.empty()) {
					inputText.pop_back();
				}	
			}
			if ((keyInputQueue.front().key == GLFW_KEY_ENTER) && (keyInputQueue.front().action == GLFW_PRESS)) {
				
				getInput = false;
				
				//Send finished input
				std::cout << "Sending Packet" << std::endl;
				client.ParseUserInput(inputText);
			}

		}
		if (getInput == false)
		{
			//Clear input buffer
			inputText.clear();

		}


		keyInputQueue.pop();
	}

	int JuicyEngine::renderLoop()
	{

		//Launch seperate thread for recieving packets;
		std::future<void> recmsg = std::async(std::launch::async, &BasicChatClient::Run, &client);

		while (!glfwWindowShouldClose(window))
		{
		
			//----------Get Delta Time---------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			//----------------------------------------

			//input
			processInput(window);

			//-----------Clear Current Frame-----------
			glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer_ID);
			glEnable(GL_DEPTH_TEST);
			clearFrameBuffer();
			//-----------------------------------------

			if (!client.message_log.empty()) {
				for (int i = 0; i < MAX_ROWS-1; i++) {
					if (i < client.message_log.size()) {


						if (client.message_log[i].size() > MAX_COLS) {
							text->DrawString(client.message_log[i].substr(0,MAX_COLS).c_str(),
								10.0f,
								(i * (text->FONT_MAX_HEIGHT + LINE_SPACING)) + LINE_SPACING,
								1.0f,
								glm::vec3(0.5f, 0.8f, 0.2f));
							i++;
							text->DrawString(client.message_log[i - 1].substr(MAX_COLS, client.message_log[i-1].size()-1).c_str(),
								10.0f,
								(i * (text->FONT_MAX_HEIGHT + LINE_SPACING)) + LINE_SPACING,
								1.0f,
								glm::vec3(0.5f, 0.8f, 0.2f));
						}
						else {
							text->DrawString(client.message_log[i].c_str(),
								10.0f,
								(i * (text->FONT_MAX_HEIGHT + LINE_SPACING)) + LINE_SPACING,
								1.0f,
								glm::vec3(0.5f, 0.8f, 0.2f));
						}


					}
					else {

					}
					
				}
			}


			if (getInput == true) {

				text->DrawString(inputText.c_str(), 10.0f , window_height - text->FONT_MAX_HEIGHT-5.0f,1.0f, default_font_color);
				text->DrawCursor(1.0f, default_font_color);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			clearFrameBuffer();

			screenShader->activate();
			glBindVertexArray(VAO_SCREEN);
			glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;





	}

	void JuicyEngine::run()
	{

		renderLoop();

	}

	static std::string loadFile(const char* path)
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

	void JuicyEngine::clearFrameBuffer()
	{

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	void JuicyEngine::addPrimative(Shader* s, primativeType p)
	{

		Primative* pim = new Primative(s, JuicyEngine::primativeType::TRIANGLE, &model, &view, &projection);

		primShapes.push_back(pim);

	}

	void JuicyEngine::addPrimative(primativeType p, std::string vertPath, std::string fragPath)
	{

		ShaderList.push_back(new Shader(vertPath.c_str(), fragPath.c_str()));

		Primative* pim = new Primative(ShaderList.back(), p, &model, &view, &projection);

		primShapes.push_back(pim);
	}

	void JuicyEngine::addPrimative(primativeType p, const char* vertPath, const char* fragPath)
	{
		std::string vert = loadFile(vertPath);

		std::string frag = loadFile(fragPath);

		addPrimative(p, vert, frag);

	}

	void JuicyEngine::SetUpFrameBuffer()
	{
		if (screenShader != nullptr) {
			//Remove old Buffers when window resizes
			glDeleteFramebuffers(1, &FrameBuffer_ID);
			glDeleteTextures(1, &textureColorBuffer);
			glDeleteRenderbuffers(1, &RBO_SCREEN);
			glDeleteVertexArrays(1, &VAO_SCREEN);
			glDeleteBuffers(1, &VBO_SCREEN);
			delete screenShader;
		}

		screenShader = new Shader("screenShader.vert", "screenShader.frag");

		// screen quad VAO
		glGenVertexArrays(1, &VAO_SCREEN);
		glGenBuffers(1, &VBO_SCREEN);

		glBindVertexArray(VAO_SCREEN);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_SCREEN);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


		//framebuffer configuration
		// -------------------------

		glGenFramebuffers(1, &FrameBuffer_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer_ID);
		// create a color attachment texture

		glGenTextures(1, &textureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

		glGenRenderbuffers(1, &RBO_SCREEN);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO_SCREEN);
		// use a single renderbuffer object for both a depth AND stencil buffer.
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_SCREEN); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//------------------------------------------------------------
	}





}



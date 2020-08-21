#include "primative.h"
namespace JuicyEngineNS
{
	float squareverts[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int square_indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};


	float triverts[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};




	float texCoordss[] = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};



	float simple_square_vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int simple_square_indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	float simple_cube_vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	Primative::Primative(Shader* shad, JuicyEngine::primativeType s, glm::mat4* m, glm::mat4* v, glm::mat4* p)
	{

		shader = shad;
		shape = s;
		model = *m;
		view = v;
		projection = p;


		switch (s) {
		case JuicyEngine::primativeType::TRIANGLE:
		{

			//-----------------------------Add triangle-------------------------------------------------
			glGenVertexArrays(1, &VAO_ID);
			glGenBuffers(1, &VBO_ID);
			glBindVertexArray(VAO_ID);
			//-----------------Bind new Object 2--------------------------------
			glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(triverts), triverts, GL_STATIC_DRAW);
			//----------------------------------------------------------------
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			//Clear binds and enables
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			break;

		}
		case JuicyEngine::primativeType::CUBE:
		{
			glGenVertexArrays(1, &VAO_ID);
			glGenBuffers(1, &VBO_ID);

			glBindVertexArray(VAO_ID);

			glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(simple_cube_vertices), simple_cube_vertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// texture coord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			tex01 = new Texture("I:\\Programming\\SpaceAdventure\\resources\\images\\coaltexture.tga", true);
			shader->Activate();
			std::string te = "texture1";
			shader->SetUniformInt(te, 0);

			break;

		}
		case JuicyEngine::primativeType::SQUARE:
		{

			glGenVertexArrays(1, &VAO_ID);
			glGenBuffers(1, &VBO_ID);
			glGenBuffers(1, &EBO_ID);

			glBindVertexArray(VAO_ID);

			glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(simple_square_vertices), simple_square_vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(simple_square_indices), simple_square_indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);


			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			tex01 = new Texture("I:\\Programming\\SpaceAdventure\\resources\\images\\coaltexture.tga", true);
			shader->Activate();
			std::string te = "texture1";
			shader->SetUniformInt(te, 0);

		}
		}



	}

	Primative::~Primative()
	{

		glDeleteVertexArrays(1, &VAO_ID);
		glDeleteBuffers(1, &VBO_ID);
		glDeleteBuffers(1, &EBO_ID);


	}

	void Primative::drawShape()
	{



		switch (shape) {
		case JuicyEngine::primativeType::TRIANGLE:
		{

			shader->Activate();
			updateTransform();
			glBindVertexArray(VAO_ID);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			break;
		}
		case JuicyEngine::primativeType::CUBE:
		{

			tex01->activate(0);
			shader->Activate();
			updateTransform();

			glBindVertexArray(VAO_ID);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;

		}

		case JuicyEngine::primativeType::SQUARE:
		{

			tex01->activate(0);
			shader->Activate();
			updateTransform();
			glBindVertexArray(VAO_ID);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			break;

		}
		}





	}

	void Primative::updateTransform()
	{
		shader->Activate();
		std::string mT = "model";
		int modelLoc = glGetUniformLocation(shader->id, "model");
		glad_glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		std::string vT = "view";
		int modelLoc1 = glGetUniformLocation(shader->id, "view");
		glad_glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(*view));
		std::string pT = "projection";
		int modelLoc2 = glGetUniformLocation(shader->id, "projection");
		glad_glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(*projection));

	}

	void Primative::bounce()
	{


		currentTrans = glm::mat4(1.0f);
		currentTrans = glm::translate(currentTrans, std::sin((float)glfwGetTime()) * glm::vec3(0.5f, -0.5f, 0.0f));
		currentTrans = glm::rotate(currentTrans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	}

	void Primative::moveTo(glm::vec3 v)
	{

		glm::mat4 newModel = glm::mat4(1.0f);
		newModel = glm::translate(newModel, v);
		float angle = 20.0f;
		newModel = glm::rotate(newModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = newModel;

	}

	void Primative::setCameraMatrices(glm::mat4& m, glm::mat4& v, glm::mat4& p)
	{

		model = m;
		view = &v;
		projection = &p;
	}


}
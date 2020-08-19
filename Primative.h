#pragma once
#include "JuicyEngine.h"

namespace JuicyEngineNS
{

	class Primative
	{
	public:
		Primative(Shader* shad, JuicyEngine::primativeType s, glm::mat4* m, glm::mat4* v, glm::mat4* p);
		~Primative();

		Shader* shader;
		JuicyEngine::primativeType shape;
		glm::mat4 transform;
		unsigned int VAO_ID;
		unsigned int VBO_ID;
		unsigned int EBO_ID;
		Texture* tex01;
		std::vector<Texture*> textures;
		void drawShape();
		void updateTransform();
		glm::mat4 currentTrans;
		void bounce();

		void moveTo(glm::vec3 v);
		void setCameraMatrices(glm::mat4& m, glm::mat4& v, glm::mat4& p);


		glm::mat4 model;
		glm::mat4* view;
		glm::mat4* projection;
	};

}
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
		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;
		Texture* tex01;
		std::vector<Texture*> textures;
		void DrawShape();
		void UpdateTransform();
		glm::mat4 currentTrans;
		void Bounce();

		void MoveTo(glm::vec3 v);
		void SetCameraMatrices(glm::mat4& m, glm::mat4& v, glm::mat4& p);


		glm::mat4 model;
		glm::mat4* view;
		glm::mat4* projection;
	};

}
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include "Texture.h"
#include "Shader.h"

using namespace std;

#define MAX_BONE_INFLUENCE 4

class Mesh {
public:

	vector<glm::vec3> position;
	vector<glm::vec3> normal;
	vector<glm::vec2> textCoord;

	vector<Texture> textures;
	Mesh(vector<glm::vec3> position, vector< glm::vec3> normal, vector<glm::vec2> textCoord, vector<Texture> textures) {
		this->position = position;
		this->normal = normal;
		this->textCoord = textCoord;
		this->textures = textures;
		setupMesh();
	}

	~Mesh() {};
	
	void Draw(Shader& shader) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			string number;
			string name = textures[i].GetType();

			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular") {
				number = std::to_string(specularNr++);
			}

			glBindTexture(GL_TEXTURE_2D, textures[i].GetTextureId());
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(VAO);
		glVertexPointer(3, GL_TRIANGLES, 0, &position);
		// Bind and enable vertex position buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Bind and enable vertex normal buffer Bind and enable vertex texture coordinate buffer
		if (normal.size() > 0 && textCoord.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		else if (normal.size() == 0 && textCoord.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		else if (normal.size() > 0 && textCoord.size() == 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		

		glDrawArrays(GL_TRIANGLES, 0, position.size());

		// Disable vertex attribute arrays
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}


private:
	unsigned int VAO;
	unsigned int VBO_V, VBO_N, VBO_T;

	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_V);
		glGenBuffers(1, &VBO_N);
		glGenBuffers(1, &VBO_T);
	

		glBindVertexArray(VAO);

		//vertex position

		glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
		glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec3), &position[0],GL_STATIC_DRAW);
		cout << position.size();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glDisableVertexAttribArray(0);

		//vertex normals
		if (normal.size() > 0 && textCoord.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
			glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glDisableVertexAttribArray(1);

			//vertex texture coords
			glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
			glBufferData(GL_ARRAY_BUFFER, textCoord.size() * sizeof(glm::vec2), &textCoord[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glDisableVertexAttribArray(2);

			glBindVertexArray(0);
		}
		else if (normal.size() == 0 && textCoord.size() > 0)
		{
			//vertex texture coords
			glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
			glBufferData(GL_ARRAY_BUFFER, textCoord.size() * sizeof(glm::vec2), &textCoord[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glDisableVertexAttribArray(1);

			glBindVertexArray(0);
		}	
		else if (normal.size() > 0 && textCoord.size() == 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
			glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glDisableVertexAttribArray(1);
		}
	}
};



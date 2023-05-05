#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>
//#include "Vertex.h"
//#include "Texture.h"
#include "Shader.h"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoord;
	//glm::vec3 tangent;
	//glm::vec3 biTangent;

	//int m_BoneIds[MAX_BONE_INFLUENCE];
	//float m_Weigths[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};


class Mesh {
public:

	vector<Vertex> vertices;
	vector<unsigned int> indices;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;

		setupMesh();
	}

	~Mesh() {};
	
	void Draw(Shader& shader) {
		//Draw mesh

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


private:
	unsigned int VAO, VBO, EBO;
	

	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//vertex position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		//vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		//vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoord));

		glBindVertexArray(0);
	}
};



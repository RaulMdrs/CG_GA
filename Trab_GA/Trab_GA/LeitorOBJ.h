#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

using namespace std;

static Mesh loadOBJ(const char* filename)
{
	// vector<Vertex> vertices;
	vector<glm::vec3> vertex_pos;
	vector<glm::vec2> vertex_texCoord;
	vector<glm::vec3> vertex_normal;

	// Vetores temporarios
	vector<glm::vec3> vertex_pos_temp;
	vector<glm::vec2> vertex_texCoord_temp;
	vector<glm::vec3> vertex_normal_temp;

	// indices
	vector<GLint> vertex_pos_indi;
	vector<GLint> vertex_texCoord_indi;
	vector<GLint> vertex_normal_indi;

	// indices temporarios 
	vector<GLint> vertex_pos_indi_temp;
	vector<GLint> vertex_texCoord_indi_temp;
	vector<GLint> vertex_normal_indi_temp;

	vector<Texture> textures;

	bool temNormal = false;
	bool temTexCoord = false;

	int countLines = 0;
	int counter = 0;

	vector<int> quadriLine;
	GLint temp_glint = 0;
	int counterDoCounter = 0;
	stringstream ss;
	std::ifstream in_file(filename);
	std::string line = "";
	std::string prefix = "";

	while (std::getline(in_file, line)) {


		ss.clear();
		ss.str(line);

		ss >> prefix;
		glm::vec3 vector;

		if (prefix == "v") {
			ss >> vector.x >> vector.y >> vector.z;
			vertex_pos_temp.push_back(vector);
		}
		else if (prefix == "vt") {
			glm::vec2 vec;
			ss >> vec.x >> vec.y;

			vertex_texCoord_temp.push_back(vec);

			temTexCoord = true;
		}
		else if (prefix == "vn") {
			ss >> vector.x >> vector.y >> vector.z;

			vertex_normal_temp.push_back(vector);

			temNormal = true;
		}

		if (prefix == "f") {
			counter = 0;
			while (ss >> temp_glint)
			{
				if (counter > 2)
				{
					counter = 0;
				}

				if (temTexCoord && temNormal)
				{
					if (counter == 0)
					{
						counterDoCounter++;
						vertex_pos_indi_temp.push_back(temp_glint - 1);
					}
					if (counter == 1) {
						vertex_texCoord_indi_temp.push_back(temp_glint - 1);

					}
					else if (counter == 2) {
						vertex_normal_indi_temp.push_back(temp_glint - 1);
					}
				}
				else if (!temTexCoord && temNormal)
				{
					if (counter == 0)
					{
						counterDoCounter++;
						vertex_pos_indi_temp.push_back(temp_glint - 1);
					}
					if (counter == 2) {
						vertex_normal_indi_temp.push_back(temp_glint - 1);
					}
				}
				else if (temTexCoord && !temNormal)
				{
					if (counter == 0)
					{
						counterDoCounter++;
						vertex_pos_indi_temp.push_back(temp_glint - 1);
					}
					if (counter == 2) {
						vertex_texCoord_indi_temp.push_back(temp_glint - 1);

					}
				}
				else if (!temTexCoord && !temNormal)
				{
					counterDoCounter++;
					vertex_pos_indi_temp.push_back(temp_glint - 1);
				}


				if (ss.peek() == '/')
				{
					counter++;
					ss.ignore(1, ' ');

					if (ss.peek() == '/')
					{
						counter++;
						ss.ignore(1, ' ');
					}
				}
				else if (ss.peek() == ' ')
				{
					counter++;
					ss.ignore(1, ' ');
				}

			}

			if (counterDoCounter == 4) {
				for (unsigned int i = 0; i < 3; i++) {
					if (temTexCoord && temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
						vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
						vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
					}
					else if (temTexCoord && !temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
						vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
					}
					else if (!temTexCoord && temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
						vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
					}
					else if (!temTexCoord && !temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
					}
				}

				for (unsigned int i = 0; i <= 3; i++) {
					if (i != 1) {
						if (temTexCoord && temNormal)
						{
							vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
							vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
							vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
						}
						else if (temTexCoord && !temNormal)
						{
							vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
							vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
						}
						else if (!temTexCoord && temNormal)
						{
							vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
							vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
						}
						else if (!temTexCoord && !temNormal)
						{
							vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
						}
					}
				}

				counterDoCounter = 0;
			}
			else {
				for (unsigned int i = 0; i < 3; i++) {

					if (temTexCoord && temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
						vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
						vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
					}
					else if (temTexCoord && !temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
						vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
					}
					else if (!temTexCoord && temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
						vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
					}
					else if (!temTexCoord && !temNormal)
					{
						vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
					}
					
				}

				counterDoCounter = 0;
			}

			
			vertex_texCoord_indi_temp.clear();
			vertex_normal_indi_temp.clear();
			vertex_pos_indi_temp.clear();
		}

		countLines++;
	}
	in_file.close();



	for (unsigned int i = 0; i < vertex_pos_indi.size(); i++)
	{
		vertex_pos.push_back(vertex_pos_temp[vertex_pos_indi[i]]);
		if (temNormal)
		{
			vertex_normal.push_back(vertex_normal_temp[vertex_normal_indi[i]]);
		}
		else
		{
			vertex_normal.clear();
		}
	}

	if (temTexCoord)
	{
		for (unsigned int i = 0; i < vertex_texCoord_indi.size(); i++)
		{
			vertex_texCoord.push_back(vertex_texCoord_temp[vertex_texCoord_indi[i]]);
		}

	}
	else
	{
		vertex_texCoord.clear();
	}

	return Mesh(vertex_pos, vertex_normal, vertex_texCoord, textures);

}

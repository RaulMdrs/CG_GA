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

    bool tudoFuiLido = false;
    int countLines = 0;
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

            //cout << vector.x << " " << vector.y << " " << vector.z << endl;

            vertex_pos_temp.push_back(vector);

            //cout << vertex_pos_temp[0].x << " " << vertex_pos_temp[0].y << " " << vertex_pos_temp[0].z << endl;
        }
        else if (prefix == "vt") {
            glm::vec2 vec;
            ss >> vec.x >> vec.y;

            vertex_texCoord_temp.push_back(vec);


            tudoFuiLido = true;

        }
        else if (prefix == "vn") {
            ss >> vector.x >> vector.y >> vector.z;

            vertex_normal_temp.push_back(vector);

            tudoFuiLido = true;

        }

        if (prefix == "f") {
            int counter = 0;
            while (ss >> temp_glint)
            {
                if (counter > 2)
                {
                    counter = 0;
                }

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

                if (ss.peek() == '/')
                {
                    counter++;
                    /*vertex_pos_.push_back(temp_glint);*/
                    ss.ignore(1, ' ');
                }
                else if (ss.peek() == ' ')
                {
                    counter++;
                    ss.ignore(1, ' ');
                }
                
            }

            if (counterDoCounter == 4) {

                ///teste

                //for (int i = 0; i < 2; i++) {
                //    if (i)
                //}

                ///
                for (unsigned int i = 0; i < 3; i++) {
                    vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
                    vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
                    vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
                }

                for (unsigned int i = 0; i <= 3; i++) {
                    if (i != 1) {
                        vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
                        vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
                        vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
                    }
                }

                counterDoCounter = 0;
            }
            else {
                for (unsigned int i = 0; i < 3; i++) {
                    vertex_pos_indi.push_back(vertex_pos_indi_temp[i]);
                    vertex_normal_indi.push_back(vertex_normal_indi_temp[i]);
                    vertex_texCoord_indi.push_back(vertex_texCoord_indi_temp[i]);
                }
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
        vertex_normal.push_back(vertex_normal_temp[vertex_normal_indi[i]]);
        //vertex_texCoord.push_back(vertex_texCoord_temp[vertex_texCoord_indi[i]]);

        /*cout << "Passei a " << i << " vez pelo for" << endl;
        cout << "Coloquei o indice do vertex_pos_indi: " << vertex_pos_indi[i] << " que e o vetor: " << vertex_pos_temp[vertex_pos_indi[i]].x 
            << " " << vertex_pos_temp[vertex_pos_indi[i]].y << " " << vertex_pos_temp[vertex_pos_indi[i]].z << endl;*/
    }
    for (unsigned int i = 0; i < vertex_texCoord_indi.size(); i++)
    {
        vertex_texCoord.push_back(vertex_texCoord_temp[vertex_texCoord_indi[i]]);

        /*cout << "Passei a " << i << " vez pelo for" << endl;
        cout << "Coloquei o indice do vertex_texCoord_indi: " << vertex_texCoord_indi[i] << " que e o vetor: " << vertex_texCoord_temp[vertex_texCoord_indi[i]].x
            << " " << vertex_texCoord_temp[vertex_texCoord_indi[i]].y << endl;*/
    }

    return Mesh(vertex_pos, vertex_normal, vertex_texCoord, textures);

}

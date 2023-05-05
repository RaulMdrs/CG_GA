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
    vector<Vertex> vertices;
	vector<glm::fvec3> vertex_pos;
	vector<glm::fvec2> vertex_texCoord;
	vector<glm::fvec3> vertex_normal;

    vector<unsigned int> indices;
	vector<GLint> vertex_pos_indi;
	vector<GLint> vertex_texCoord_indi;
	vector<GLint> vertex_normal_indi;
    
    vector<Texture> texture;

    bool tudoFuiLido = false;

    GLint temp_glint = 0;

	stringstream ss;
	std::ifstream in_file(filename);
    std::string line = "";
    std::string prefix = "";

    while (std::getline(in_file, line)) {
        ss.clear();
        ss.str(line);

        ss >> prefix;
        Vertex vertex;
        glm::vec3 vector;

        if (prefix == "v") {
            ss >> vector.x >> vector.y >> vector.z;

            vertex.position = vector;
            vertex_pos.push_back(vector);
           
            tudoFuiLido = true;
          
            cout << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << endl;

        }
        else if (prefix == "vt") {
            glm::vec2 vec;
            ss >> vec.x >> vec.y;

            vertex_texCoord.push_back(vec);

            vertex.textCoord = vec;

            tudoFuiLido = true;

            cout << vertex.textCoord.x << " " << vertex.textCoord.y << " " << endl;
        }
        else if (prefix == "vn") {
            ss >> vector.x >> vector.y >> vector.z;
            vertex.normal = vector;

            vertex_normal.push_back(vector);


            tudoFuiLido = true;

            cout << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << endl;
        }

        if (tudoFuiLido)
        {
            vertices.push_back(vertex);
            tudoFuiLido = false;
        }

        if (prefix == "f") {
            int counter = 0;
            while (ss >> temp_glint)
            {

                if (counter > 11)
                {
                    cout << endl;
                }
                cout << temp_glint << " " << endl;;

                if (counter == 0)
                {
                    vertex_pos_indi.push_back(temp_glint);
                }
                else if (counter == 1)
                {
                    vertex_texCoord_indi.push_back(temp_glint);

                }
                else if (counter == 2)
                {
                    vertex_normal_indi.push_back(temp_glint);

                }

                if (ss.peek() == '/')
                {
                    counter++;
                    ss.ignore(1, ' ');
                }
                else if (ss.peek() == ' ')
                {
                    counter++;
                    ss.ignore(1, ' ');
                }

                //if (counter > 2)
                //{
                //    counter = 0;
                //}
            }


           /* vertices.resize(vertex_pos_indi.size(), Vertex());

            for (size_t i = 0; i < vertices.size(); i++)
            {
                vertices[i].position = vertex_pos[vertex_pos_indi[i]];
                vertices[i].normal = vertex_normal[vertex_normal_indi[i]];
                vertices[i].textCoord = vertex_texCoord[vertex_texCoord_indi[i]];
            }*/

            //std::string faceToken;
            //std::vector<std::string> faceTokens;
            //while (ss >> faceToken) {
            //    faceTokens.push_back(faceToken);
            //}
            //for (unsigned int i = 0; i < faceTokens.size(); i++) {
            //    std::string vertexToken = faceTokens[i];
            //    std::istringstream vertexIss(vertexToken);
            //    std::string vertexIndexToken;
            //    std::vector<std::string> vertexIndexTokens;
            //    while (std::getline(vertexIss, vertexIndexToken, '/')) {
            //        vertexIndexTokens.push_back(vertexIndexToken);
            //    }
            //    int vertexIndex = std::stoi(vertexIndexTokens[0]) - 1;
            //    int texCoordIndex = std::stoi(vertexIndexTokens[1]) - 1;
            //    int normalIndex = std::stoi(vertexIndexTokens[2]) - 1;


            //    indices.push_back(vertexIndex);
            //    //indices.push_back(normalIndex);
            //    //indices.push_back(texCoordIndex);
            //}
        }
    }
    in_file.close();

    std::cout << "Vertices Tam:" << vertices.size() << std::endl;

    std::cout << "Vertices:" << std::endl;

    

    for (unsigned int i = 0; i < vertices.size(); i++) {

        if (i < 8)
        {

            std::cout << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << std::endl;
        }
        else if (i < 22)
        {
            std::cout << vertices[i].textCoord.x << ", " << vertices[i].textCoord.y << std::endl;
        }
        else
        {
            std::cout << vertices[i].normal.x << ", " << vertices[i].normal.y << ", " << vertices[i].normal.z << std::endl;
        }
    }

    // imprime a lista de faces
    //std::cout << "Faces:" << std::endl;
    //for (unsigned int i = 0; i < indices.size(); i += 3) {
    //    std::cout << indices[i] << ", " << indices[i + 1] << ", " << indices[i + 2] << ", " << std::endl;
    //}

    /*  std::cout << "Texture:" << std::endl;
    for (unsigned int i = 0; i < objeto.textures.size(); i += 4) {
        std::cout << objeto.textures[i] << ", " << objeto.textures[i + 1] << ", " << objeto.textures[i + 2] << ", " << objeto.textures[i + 3] << std::endl;
    }*/


    return Mesh(vertices, indices);

}

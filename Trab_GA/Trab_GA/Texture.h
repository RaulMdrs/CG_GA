#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <String>
#include "AssetManager.h"

using namespace std;

class Texture
{
private:
	unsigned int textureID;
	GLint textureLocation;
	GLshort textureNum;
	string type;

public:
	string path;

	Texture();
	Texture(char* path, char* textureUniformName, GLuint shaderProgram, GLint textureNum);
	~Texture();

	string GetType() { return type; }
	GLuint GetTextureId() { return textureID; }
	GLshort GetTextureNum() { return textureNum; }

	void SetID(unsigned int id) { textureID = id; }
	void SetType(string tp) { type = tp; }
};

#endif
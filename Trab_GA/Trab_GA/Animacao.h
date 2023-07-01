#pragma once
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Animacao
{

	vector<glm::vec3> animacaoPoints;

public:
	Animacao();
	~Animacao();

	void LerPoints(char* path);
	vector<glm::vec3> GetAnimacaoPoints();
};


#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>

using namespace std;

class Material
{
	string id;
	vector<glm::vec3> ka, kd, ks;
	float Ns;
	string map_kd;
};
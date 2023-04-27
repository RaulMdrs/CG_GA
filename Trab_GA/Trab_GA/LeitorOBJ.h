#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <glm\glm.hpp>

using namespace std;

class LeitorOBJ
{

public:
	LeitorOBJ();
	~LeitorOBJ();

	//Obj LerOBJ(std::string name);
	bool lerArquivo(const std::string& filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices);
};
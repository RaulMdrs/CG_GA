#include "Animacao.h"

Animacao::Animacao()
{
	animacaoPoints.clear();
}

Animacao::~Animacao()
{
}

void Animacao::LerPoints(char* path)
{
	stringstream ss;
	std::ifstream in_file(path);
	std::string line = "";
	std::string prefix = "";

	glm::vec3 tempPoints;

	while (std::getline(in_file, line))
	{
		ss.clear();
		ss.str(line);

		ss >> tempPoints.x >> tempPoints.y >> tempPoints.z;

		animacaoPoints.push_back(tempPoints);
	}

	in_file.close();
}

vector<glm::vec3> Animacao::GetAnimacaoPoints()
{
	return animacaoPoints;
}

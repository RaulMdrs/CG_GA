#include "LeitorOBJ.h"

LeitorOBJ::LeitorOBJ()
{
}

LeitorOBJ::~LeitorOBJ()
{
}

Obj LeitorOBJ::LerOBJ(std::string name)
{
    Obj objeto;

    std::string filename = name + ".obj";

    // abre o arquivo OBJ para leitura
    std::ifstream objFile(filename);

    if (!objFile.is_open()) {
        std::cerr << "Erro ao abrir arquivo OBJ" << std::endl;
    }

    // lê cada linha do arquivo
    std::string line;
    while (std::getline(objFile, line)) {
        // separa a linha em palavras
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "v") {
            // a linha é um vértice, adiciona os valores ao vetor de vértices
            GLfloat x, y, z;
            ss >> x >> y >> z;
            objeto.vertices.push_back(x);
            objeto.vertices.push_back(y);
            objeto.vertices.push_back(z);
        }
        else if (keyword == "f") {
            // a linha é uma face, adiciona os índices dos vértices ao vetor de faces
            int vIndex, vtIndex, vnIndex;
            char slash;
            while (ss >> vIndex >> slash >> vtIndex >> slash >> vnIndex) {

                objeto.faces.push_back(vIndex);
                objeto.textures.push_back(vtIndex);
                objeto.normals.push_back(vnIndex);
            }
        }

    }

    // imprime a lista de vértices
    std::cout << "Vertices:" << std::endl;
    for (unsigned int i = 0; i < objeto.vertices.size(); i += 3) {
        std::cout << objeto.vertices[i] << ", " << objeto.vertices[i + 1] << ", " << objeto.vertices[i + 2] << std::endl;
    }

    // imprime a lista de faces
    std::cout << "Faces:" << std::endl;
    for (unsigned int i = 0; i < objeto.faces.size(); i += 4) {
        std::cout << objeto.faces[i] << ", " << objeto.faces[i + 1] << ", " << objeto.faces[i + 2] << ", " << objeto.faces[i + 3] << std::endl;
    }

    std::cout << "Texture:" << std::endl;
    for (unsigned int i = 0; i < objeto.textures.size(); i += 4) {
        std::cout << objeto.textures[i] << ", " << objeto.textures[i + 1] << ", " << objeto.textures[i + 2] << ", " << objeto.textures[i + 3] << std::endl;
    }

    return objeto;
}

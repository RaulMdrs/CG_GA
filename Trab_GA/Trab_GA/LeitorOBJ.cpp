#include "LeitorOBJ.h"

LeitorOBJ::LeitorOBJ()
{
}

LeitorOBJ::~LeitorOBJ()
{
}

bool LeitorOBJ::lerArquivo(const std::string& filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices)
{
    cout << "\n leitor";

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::string token;
        iss >> token;

        if (token == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (token == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (token == "vt") {
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (token == "f") {
            std::string faceToken;
            std::vector<std::string> faceTokens;
            while (iss >> faceToken) {
                faceTokens.push_back(faceToken);
            }

            // Processa cada vértice da face
            for (unsigned int i = 0; i < faceTokens.size(); i++) {
                std::string vertexToken = faceTokens[i];
                std::istringstream vertexIss(vertexToken);

                std::string vertexIndexToken;
                std::vector<std::string> vertexIndexTokens;
                while (std::getline(vertexIss, vertexIndexToken, '/')) {
                    vertexIndexTokens.push_back(vertexIndexToken);
                }

                int vertexIndex = std::stoi(vertexIndexTokens[0]) - 1;
                int texCoordIndex = std::stoi(vertexIndexTokens[1]) - 1;
                int normalIndex = std::stoi(vertexIndexTokens[2]) - 1;

                indices.push_back(indices.size());
                vertices.push_back(vertices[vertexIndex]);
                normals.push_back(normals[normalIndex]);
                texCoords.push_back(texCoords[texCoordIndex]);
            }
        }
    }

    file.close();

    if (vertices.size() == 0 || indices.size() == 0) {
        return false;
    }

    return true;
}

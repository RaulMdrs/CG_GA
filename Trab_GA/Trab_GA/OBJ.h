#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <GL\glew.h>
#include <glm/glm.hpp>
#include "LeitorOBJ.h"

class OBJ {
public:
    OBJ() {}

    bool carregarArquivo(const std::string& filename) {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCoords;
        std::vector<unsigned int> indices;

        LeitorOBJ leitorOBJ;
        bool resultado = leitorOBJ.lerArquivo(filename, vertices, normals, textureCoords, indices);

        if (resultado) {
            m_Vertices = vertices;
            m_Indices = indices;
            return true;
        }

        return false;
    }

    const std::vector<glm::vec3>& getVertices() const {
        return m_Vertices;
    }

    const std::vector<unsigned int>& getIndices() const {
        return m_Indices;
    }

private:
    std::vector<glm::vec3> m_Vertices;
    std::vector<unsigned int> m_Indices;
};
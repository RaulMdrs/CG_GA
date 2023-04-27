#include <glm\glm.hpp>

class Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex) :
        position(pos), normal(norm), texCoord(tex) {}
};
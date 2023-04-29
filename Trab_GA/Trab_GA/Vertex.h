#pragma once
#include <glm\glm.hpp>

#define MAX_BONE_INFLUENCE 4
struct Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textCoord;
    glm::vec3 tangent;
    glm::vec3 biTangent;

    int m_BoneIds[MAX_BONE_INFLUENCE];
    float m_Weigths[MAX_BONE_INFLUENCE];
};
#pragma once
#include <glm/glm.hpp>

enum BlockType {
    AIR = 0,
    DIRT,
    STONE,
};


struct BlockLighting {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
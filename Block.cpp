#include "Block.h"
#include <iostream>

Block::Block() {
    std::cout << "Generating default block..." << std::endl;
    type = BlockType::DIRT;
    setup();
};

Block::Block(glm::vec3 local_position, BlockType type)
    :local_position(local_position), type(type){
    setup();
}


void Block::setup() {
    color = glm::vec3(0.5f, 0.5f, 0.5f);
    lighting.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    lighting.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    lighting.specular = glm::vec3(0.2f, 0.2f, 0.2f);
    lighting.shininess = 32.0f;

    absolute_position = local_position + glm::vec3(0, 0, 0);    //TODO modify 000 with chunk position
}

void Block::setShaderLighting(Shader& shader) {
    shader.setVec3("material.ambient", lighting.ambient);
    shader.setVec3("material.diffuse", lighting.diffuse);
    shader.setVec3("material.specular", lighting.specular);
    shader.setFloat("material.shininess", lighting.shininess);
}

BlockLighting Block::getLighting() const
{
    return lighting;
}


glm::vec3 Block::getLocalPosition() const {
    return local_position;
}
glm::vec3 Block::getAbsolutePosition() const {
	return absolute_position;
}

glm::vec3 Block::getColor() const {
    return color;
}

void Block::setType(BlockType newType)
{
    type = newType;
}
BlockType Block::getType() const
{
    return type;
}
void Block::generateVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices) const {
    // Generate vertices for the block
    float x = absolute_position.x;
    float y = absolute_position.y;
    float z = absolute_position.z;
    float r = color.r;
    float g = color.g;
    float b = color.b;


    vertices = {
        // Positions      // Colors       // Texture Coords
        x, y, z,          r, g, b,       0.0f, 0.0f, // Front face
        x + 1, y, z,      r, g, b,       1.0f, 0.0f,
        x + 1, y + 1, z,  r, g, b,       1.0f, 1.0f,
        x, y + 1, z,      r, g, b,       0.0f, 1.0f,

        x, y, z + 1,      r, g, b,       0.0f, 0.0f, // Back face
        x + 1, y, z + 1,  r, g, b,       1.0f, 0.0f,
        x + 1, y + 1, z + 1, r, g, b,    1.0f, 1.0f,
        x, y + 1, z + 1,  r, g, b,       0.0f, 1.0f,

        x, y, z,          r, g, b,       0.0f, 0.0f, // Left face
        x, y, z + 1,      r, g, b,       1.0f, 0.0f,
        x, y + 1, z + 1,  r, g, b,       1.0f, 1.0f,
        x, y + 1, z,      r, g, b,       0.0f, 1.0f,

        x + 1, y, z,      r, g, b,       0.0f, 0.0f, // Right face
        x + 1, y, z + 1,  r, g, b,       1.0f, 0.0f,
        x + 1, y + 1, z + 1, r, g, b,    1.0f, 1.0f,
        x + 1, y + 1, z,  r, g, b,       0.0f, 1.0f,

        x, y, z,          r, g, b,       0.0f, 0.0f, // Bottom face
        x + 1, y, z,      r, g, b,       1.0f, 0.0f,
        x + 1, y, z + 1,  r, g, b,       1.0f, 1.0f,
        x, y, z + 1,      r, g, b,       0.0f, 1.0f,

        x, y + 1, z,      r, g, b,       0.0f, 0.0f, // Top face
        x + 1, y + 1, z,  r, g, b,       1.0f, 0.0f,
        x + 1, y + 1, z + 1, r, g, b,    1.0f, 1.0f,
        x, y + 1, z + 1,  r, g, b,       0.0f, 1.0f
     };
        
    indices = {
        0, 1, 2, 0, 2, 3,   // Front face
        4, 5, 6, 4, 6, 7,   // Back face
        8, 9, 10, 8, 10, 11, // Left face
        12, 13, 14, 12, 14, 15, // Right face
        16, 17, 18, 16, 18, 19, // Bottom face
        20, 21, 22, 20, 22, 23  // Top face
    };

    //vertices = {
    //    // Positions      // Colors       // Texture Coords
    //    x, y, z,          r, g, b,       0.0f, 0.0f,
    //    x + 1, y, z,      r, g, b,       1.0f, 0.0f,
    //    x + 1, y + 1, z,  r, g, b,       1.0f, 1.0f,
    //    x, y + 1, z,      r, g, b,       0.0f, 1.0f,
    //    x, y, z + 1,      r, g, b,       0.0f, 0.0f,
    //    x + 1, y, z + 1,  r, g, b,       1.0f, 0.0f,
    //    x + 1, y + 1, z + 1, r, g, b,    1.0f, 1.0f,
    //    x, y + 1, z + 1,  r, g, b,       0.0f, 1.0f,
    //};

    //// Generate indices for the block
    //indices = {
    //    0, 1, 2, 2, 3, 0,
    //    4, 5, 6, 6, 7, 4,
    //    0, 1, 5, 5, 4, 0,
    //    2, 3, 7, 7, 6, 2,
    //    0, 3, 7, 7, 4, 0,
    //    1, 2, 6, 6, 5, 1
    //};
}

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <vector>
#include "structures.h"
#include <shader.h>


class Block {
public:
    Block();
    Block(glm::vec3 local_position, BlockType type);
    
    glm::vec3 getLocalPosition() const;
    glm::vec3 getAbsolutePosition() const;
    glm::vec3 getColor() const;

    void generateVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices) const;
    void setType(BlockType newType);
    BlockType getType() const;
    void setShaderLighting(Shader& shader);
    void setup();
    BlockLighting getLighting() const;
private:
    glm::vec3 absolute_position;
    glm::vec3 local_position;
    glm::vec3 color;
    BlockType type;
    BlockLighting lighting;
};

#endif // BLOCK_H

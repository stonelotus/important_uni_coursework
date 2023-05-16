#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <vector>
#include <iostream>
class Chunk {
public:
    Chunk();
    Chunk(glm::vec3 sizes);
    ~Chunk();
    const Block& getBlock(glm::vec3 block_position) const;
    void setBlock(glm::vec3 block_position, const Block& block);

  
    void removeBlock(int position);
    void generateVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices) const;
    BlockLighting calculateAverageMaterialProperties() const;

    void setup();
    glm::vec3 getPosition() const;
    glm::vec3 getSize();

private:
    glm::vec3 sizes;
    std::vector<std::vector<std::vector<Block>>> blocks;
    glm::vec3 origin_position;
};

#endif // CHUNK_H

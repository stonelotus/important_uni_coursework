#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <vector>
#include <iostream>
#include "BoundingBox.h"
class Chunk {
public:
    Chunk();
    Chunk(glm::vec3 sizes,glm::vec3 origin_position);
    ~Chunk();
    const Block& getBlock(glm::vec3 block_position) const;
    void setBlock(glm::vec3 block_position, const Block& block);

  
    void removeBlock(int position);
    void generateVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices) const;
    BlockLighting calculateAverageMaterialProperties() const;

    void setup();
    glm::vec3 getPosition() const;
    glm::vec3 getSize();

    BoundingBox getHitbox() const;

private:
    BoundingBox hitBox;
    glm::vec3 sizes;
    std::vector<std::vector<std::vector<Block>>> blocks;
    glm::vec3 origin_position;
};

#endif // CHUNK_H

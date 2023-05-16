#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Chunk.h"
#include "Shader.h"
#include <vector>
#include "Texture.h"
class ChunkRenderer {
public:
    ChunkRenderer(Chunk& chunk);
    ~ChunkRenderer();
    void render(Shader& shader);
    void modifiedChunk(std::string action);

private:
    Chunk& chunk;

    unsigned int VAO_dirt, VBO_dirt, EBO_dirt;
    unsigned int VAO_stone, VBO_stone, EBO_stone;
    std::vector<unsigned int> indices;

    void setupChunk(BlockType block_type, std::vector<float>& chunkIndices, std::vector<unsigned int>& chunkVertices, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
    std::vector<float> chunkVertices_dirt;
    std::vector<unsigned int> chunkIndices_dirt;

    std::vector<float> chunkVertices_stone;
    std::vector<unsigned int> chunkIndices_stone;

    Texture texture;

};

#endif // CHUNKRENDERER_H

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "FastNoiseLite.h"
#include "Chunk.h"
#include "Texture.h"
#include "ChunkRenderer.h"


class World {
public:
    World(unsigned int width, unsigned int depth);
    void generateGround();
    void draw(Shader& shader);
    std::vector<ChunkRenderer*> chunkRenderers;

private:
    std::vector<Chunk*> chunks;
    FastNoiseLite noiseGenerator;
    unsigned int width;
    unsigned int depth;
};

#endif // WORLD_H

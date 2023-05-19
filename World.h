#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "FastNoiseLite.h"
#include "Chunk.h"
#include "Texture.h"
#include "ChunkRenderer.h"
#include "Player.h"


class World {
public:
    World(unsigned int width, unsigned int depth);
    void generateGround();
    void draw(Shader& shader);
    bool checkCollisions(Player player);
    bool checkCollisions(BoundingBox);
    std::vector<ChunkRenderer*> chunkRenderers;


private:
    std::vector<Chunk*> chunks;
    FastNoiseLite noiseGenerator;
    unsigned int width;
    unsigned int depth;
};

#endif // WORLD_H

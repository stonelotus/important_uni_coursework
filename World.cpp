#include "World.h"
#include "ChunkRenderer.h"

World::World(unsigned int width, unsigned int depth) : width(width), depth(depth) {
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    generateGround();
}

void World::generateGround() {
    std::cout << "Generating ground..." << std::endl;
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int z = 0; z < depth; ++z) {
            chunks.emplace_back(new Chunk());  // 16x16x16 chunk

            
            //Chunk* chunk = chunks.back();
            //unsigned int height = 10 + noiseGenerator.GetNoise(float(x), float(z)) * 10;
            //for (unsigned int y = 0; y < height; ++y) {
            //    (*chunk).setBlock(glm::vec3(x % 16, y%16, z % 16), Block(glm::vec3(x, y, z)));
            //}
        }
    }
    for (Chunk* chunk : chunks) {
		chunkRenderers.emplace_back(new ChunkRenderer(*chunk));
	}
    std::cout << "Chunk renderers: " << chunkRenderers.size() << std::endl;
    std::cout << "Generating ground DONE =====" << std::endl;
}

void World::draw(Shader& shader) {

    for (ChunkRenderer* cR : chunkRenderers) {
        (*cR).render(shader);
    }
}

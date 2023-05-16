#include "ChunkRenderer.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
using namespace std;



ChunkRenderer::ChunkRenderer(Chunk& chunk)
    : chunk(chunk),texture(BlockType::DIRT) {
    // Set up the VAO, VBO, and EBO for the chunk
    setupChunk(BlockType::DIRT, chunkVertices_dirt, chunkIndices_dirt, VAO_dirt, VBO_dirt, EBO_dirt);
    setupChunk(BlockType::STONE, chunkVertices_stone, chunkIndices_stone, VAO_stone, VBO_stone, EBO_stone);
}

void ChunkRenderer::render(Shader& shader) {
    shader.use();

    // Calculate average material properties for the chunk
    //BlockLighting averageMaterial = chunk.calculateAverageMaterialProperties();

    //// Set material properties in the shader
    //shader.setVec3("material.ambient", averageMaterial.ambient);
    //shader.setVec3("material.diffuse", averageMaterial.diffuse);
    //shader.setVec3("material.specular", averageMaterial.specular);
    //shader.setFloat("material.shininess", averageMaterial.shininess);

    //// Here we are assuming that you have a global light source with constant properties
    //// If you have multiple lights or lights with variable properties, you'll need to update this
    //shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    //shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    //shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

   std::cout << chunkIndices_dirt.size() << std::endl;
    texture.Bind(0);
    shader.setInt("texture1", 0);
    glBindVertexArray(VAO_dirt);
    glDrawElements(GL_TRIANGLES, chunkIndices_dirt.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    texture.Unbind();

    Texture texture2(BlockType::STONE);
    texture2.Bind(0);
    shader.setInt("texture1", 0);
    glBindVertexArray(VAO_stone);
    glDrawElements(GL_TRIANGLES, chunkIndices_stone.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    texture2.Unbind();

}

void ChunkRenderer::setupChunk(BlockType block_type, std::vector<float>& chunkVertices, std::vector<unsigned int>& chunkIndices, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO) {
    chunkIndices.clear();
    chunkVertices.clear();
    std::cout << "Setting up chunk..." << std::endl;
    // Iterate through all the blocks in the chunk
    for (int x = 0; x < chunk.getSize().x; ++x) {
        for (int y = 0; y < chunk.getSize().y; ++y) {
            for (int z = 0; z < chunk.getSize().z; ++z) {
                Block block = chunk.getBlock(glm::vec3(x,y,z));
               
                if (block.getType() == BlockType::AIR) {
					continue;
				}

                if (block.getType() != block_type)
                    continue;
                std::vector<float> blockVertices;
                std::vector<unsigned int> blockIndices;
                block.generateVertices(blockVertices, blockIndices);
                
                // Update the block's indices to account for the current vertex count in the chunk
                for (unsigned int& index : blockIndices) {
                    index += chunkVertices.size() / 8;
                }
                // Append the block's vertices and indices to the chunk's data
                chunkVertices.insert(chunkVertices.end(), blockVertices.begin(), blockVertices.end());
                chunkIndices.insert(chunkIndices.end(), blockIndices.begin(), blockIndices.end());
            }
        }
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, chunkVertices.size() * sizeof(float), chunkVertices.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkIndices.size() * sizeof(unsigned int), chunkIndices.data(), GL_STATIC_DRAW);

    // Position attribute
   // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    std::cout << "Chunk setup DONE ===== " << std::endl;
}



ChunkRenderer::~ChunkRenderer() {
    glDeleteVertexArrays(1, &VAO_dirt);
    glDeleteBuffers(1, &VBO_dirt);
    glDeleteBuffers(1, &EBO_dirt);

    glDeleteVertexArrays(1, &VAO_stone);
    glDeleteBuffers(1, &VBO_stone);
    glDeleteBuffers(1, &EBO_stone);
}


void ChunkRenderer::modifiedChunk(std::string action) {
    if (action.compare("remove_block") == 0) {
        cout << "Removed block." << endl;
        chunk.removeBlock(1);
        //chunk.setSizeX(chunk.getSize().x - 1);

    }
 /*   glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);*/

    //setupChunk();
}
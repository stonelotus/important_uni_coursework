#pragma once

#include "Block.h"
#include "shader.h"
#include <vector>
#include <glad/glad.h>

class BlockRenderer {
public:
    BlockRenderer(const Block& block, Shader& shader);
    ~BlockRenderer();

    void render();

private:
    GLuint VAO, VBO, EBO;
    Shader& shader;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

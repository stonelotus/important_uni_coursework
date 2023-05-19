#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <shader.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices);

    void draw(Shader& shader) const;

private:
    std::vector<Vertex> vertices;
    GLuint vao, vbo;
};

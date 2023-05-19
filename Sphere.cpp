#include "Sphere.h"


Sphere::Sphere() {
    generateSphere(1.0f, 100, 100);
	setup();
}


void Sphere::render(Shader& shader)
{
    shader.use();
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::setup()
{   
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(GLfloat), sphere_vertices.data(), GL_STATIC_DRAW);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_indices.size() * sizeof(GLuint), sphere_indices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

}
void Sphere::generateSphere(float radius, unsigned int rings, unsigned int sectors)
{
    std::cout << "Generating sphere..." << std::endl;
    float PI = 3.14159265358979323846;
    float const R = 1.0 / (float)(rings - 1);
    float const S = 1.0 / (float)(sectors - 1);
    int r, s;

    sphere_vertices.resize(rings * sectors * 3);
    sphere_indices.resize(rings * sectors * 6);

    std::vector<GLfloat>::iterator v = sphere_vertices.begin();
    std::vector<GLuint>::iterator i = sphere_indices.begin();

    for (r = 0; r < rings; r++) {
        for (s = 0; s < sectors; s++) {
            float const y = sin(-PI / 2 + PI * r * R);
            float const x = cos(2 * PI * s * S) * sin(PI * r * R);
            float const z = sin(2 * PI * s * S) * sin(PI * r * R);

            *v++ = x * radius;
            *v++ = y * radius;
            *v++ = z * radius;

            if (r < rings - 1) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s + 1);
                *i++ = (r + 1) * sectors + (s + 1);

                *i++ = r * sectors + s;
                *i++ = (r + 1) * sectors + (s + 1);
                *i++ = (r + 1) * sectors + s;
            }
        }
    }

    std::cout << "Sphere generated." << std::endl;

    std::cout << sphere_vertices.size() << " vertices generated."  << std::endl;
    std::cout << sphere_indices.size() << " indices generated." << std::endl;
}

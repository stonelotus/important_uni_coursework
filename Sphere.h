#pragma once
#include <shader.h>
#include <vector>
#include <cmath>
#include "Sphere.h"
#include <glad/glad.h>
class Sphere {
public:
	Sphere();
	void render(Shader& shader);
	void setup();
	void generateSphere(float radius, unsigned int rings, unsigned int sectors);
private:
	unsigned int VAO, VBO;
	GLuint EBO;
	std::vector<GLfloat> sphere_vertices;
	std::vector<GLuint> sphere_indices;

};
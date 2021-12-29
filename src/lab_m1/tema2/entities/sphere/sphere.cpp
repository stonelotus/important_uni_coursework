#include "Sphere.h"

Sphere::Sphere(){}

Sphere::Sphere(dimensionsTriplet position, float radius) :x_center(position.x), y_center(position.y), z_center(position.z), radius(radius) {
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix = glm::scale(this->modelMatrix, { this->radius, this->radius, this->radius });
	this->modelMatrix = glm::translate(this->modelMatrix, { this->x_center,this->y_center,this->z_center });
}

glm::mat4 Sphere::getModelMatrix() {
	return this->modelMatrix;
}

float Sphere::getRadius() {
	return this->radius;
}
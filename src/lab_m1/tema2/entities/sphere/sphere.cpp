#include "Sphere.h"

Sphere::Sphere(){}

Sphere::Sphere(dimensionsTriplet position, float radius) :x_center(position.x), y_center(position.y), z_center(position.z), radius(radius) {
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix = glm::translate(this->modelMatrix, { this->x_center,this->y_center,this->z_center });
	//this->modelMatrix = glm::scale(this->modelMatrix, { this->radius, this->radius, this->radius });

}
Sphere::~Sphere() {};
glm::mat4 Sphere::getModelMatrix() {
	return this->modelMatrix;
}

float Sphere::getRadius() {
	return this->radius;
}
void Sphere::Move(dimensionsTriplet offset) {
	this->x_center += offset.x;
	this->y_center += offset.y;
	this->z_center += offset.z;
	// Almost forgot that moving is made with translation, rotation AND SCALE ffs..
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix = glm::translate(this->modelMatrix, { this->x_center, this->y_center, this->z_center });
	this->modelMatrix = glm::scale(this->modelMatrix, { 0.3f,0.3f,0.3f });
}

dimensionsTriplet Sphere::getPosition() {
	return { this->x_center,this->y_center,this->z_center };
}
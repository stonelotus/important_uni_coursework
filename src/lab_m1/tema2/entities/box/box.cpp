#include "box.h"


Box::Box() {}

Box::Box(dimensionsTriplet position,dimensionsTriplet size) :x(position.x), y(position.y), z(position.z) {
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix *= transform3D::Translate(position.x,position.y,position.z);
	this->modelMatrix *= transform3D::Scale(size.x, size.y, size.z);
}

Box::~Box(){}
glm::mat4 Box::getModelMatrix() {
	return this->modelMatrix;
}

void Box::setPosition(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;
	this->modelMatrix = transform3D::Translate(x,y,z) * this->modelMatrix;
}
dimensionsTriplet Box::getPosition() {
	return { x = this->x, y = this->y, z = this->z };
}
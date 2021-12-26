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

float Box::getX() {
	return this->x;
}
float Box::getY() {
	return this->y;
}
float Box::getZ() {
	return this->z;
}
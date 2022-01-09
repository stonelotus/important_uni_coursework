#include "box.h"


Box::Box() {}

Box::Box(dimensionsTriplet position,dimensionsTriplet size) :x(position.x), y(position.y), z(position.z),
															 sizeX(size.x), sizeY(size.y), sizeZ(size.z){
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix *= transform3D::Translate(position.x,position.y,position.z);
	this->modelMatrix *= transform3D::Scale(size.x, size.y, size.z);
	
	/*this->modelMatrix *= transform3D::RotateOY(this->angle);
	this->modelMatrix *= transform3D::Translate(0, 0, 0);
	*/
	//this->color = "";
}

Box::~Box(){}
glm::mat4 Box::getModelMatrix() {
	return this->modelMatrix;
}

void Box::Move(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;
	this->modelMatrix = glm::mat4(1);
	//this->modelMatrix = glm::translate(this->modelMatrix, { 0,0,0 });	//this->modelMatrix = glm::translate(this->modelMatrix, { this->x, this->y, this->z });
	this->modelMatrix = glm::translate(this->modelMatrix, { this->x, this->y, this->z });
	this->modelMatrix = glm::rotate(this->modelMatrix, this->angle, glm::vec3(0, 1, 0));
	this->modelMatrix = glm::scale(this->modelMatrix, { this->sizeX,this->sizeY, this->sizeZ });
	
}
dimensionsTriplet Box::getPosition() {
	return { x = this->x, y = this->y, z = this->z };
}

void Box::setPosition(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix = glm::scale(this->modelMatrix, { this->sizeX, this->sizeY, this->sizeZ });
	this->modelMatrix = glm::translate(this->modelMatrix, { this->x,this->y,this->z });
}

glm::vec3 Box::getShaderColor()
{
	return this->color;
}
void Box::setShaderColor(glm::vec3 new_shader_color)
{
	this->color = new_shader_color;
}

void Box::setAngle(float angle) {
	this->angle = angle;
	this->Move(0, 0, 0);

}

void Box::Rotate(float angle) {
	this->angle += angle;
}
#include "Rectangle.h"

using namespace std;


Rectangle::Rectangle(float x, float y,float sizeX, float sizeY)
{
	this->x = x;
	this->y = y;
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	this->modelMatrix = glm::mat3(1);
	this->modelMatrix *= transform2D::Translate(x, y);
	this->modelMatrix *= transform2D::Scale(sizeX, sizeY);
}
Rectangle::~Rectangle()
{

}

float Rectangle::getX()
{
	return this->x;
}
float Rectangle::getY()
{
	return this->y;

}
float Rectangle::getSizeX()
{
	return this->sizeX;
}
float Rectangle::getSizeY()
{
	return this->sizeY;
}
void Rectangle::setX(float x)
{
	this->x = x;
}
void Rectangle::setY(float y) {
	this->y = y;
}
void Rectangle::setModelMatrix(float x, float y, float sizeX, float sizeY, float rotationAngle)
{
	this->modelMatrix = glm::mat3(1);
	//this->modelMatrix *= transform2D::Scale(scale, scale);
	this->modelMatrix *= transform2D::Translate(x, y);

	this->modelMatrix *= transform2D::Translate(sizeX / 2, sizeY / 2);
	this->modelMatrix *= transform2D::Rotate(rotationAngle);
	this->modelMatrix *= transform2D::Translate(-sizeX / 2, -sizeY / 2);

}
void Rectangle::setModelMatrix(glm::mat3 modelMatrix)
{
	this->modelMatrix = modelMatrix;
}

glm::mat3 Rectangle::getModelMatrix()
{
	return this->modelMatrix;
}
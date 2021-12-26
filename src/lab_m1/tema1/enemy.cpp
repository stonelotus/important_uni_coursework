#include "enemy.h"

Enemy::Enemy(float x,float y, float scale) : body(x,y,scale,scale),
											 eye1(x,y-10,scale,scale),
											 eye2(x+80,y-10,scale,scale)
{
	
	this->x = x;
	this->y = y;
	this->speed = rand() % 400 + 50;

	this->modelMatrix = glm::mat3(1);
	this->modelMatrix *= transform2D::Translate(x, y);
	this->modelMatrix *= transform2D::Scale(scale, scale);
}

Enemy::~Enemy(){}

void Enemy::setX(float x)
{
	this->x = x;
}
void Enemy::setY(float y)
{
	this->y = y;
}
void Enemy::setRotationAngle(float rotationAngle)
{
	this->rotationAngle = rotationAngle;
}
float Enemy::getX()
{
	return this->x;
}
float Enemy::getY()
{
	return this->y;
}
float Enemy::getRotationAngle()
{
	return this->rotationAngle;
}
float Enemy::getSpeed()
{
	return this->speed;
}
void Enemy::setModelMatrix(float x, float y, float rotationAngle)
{
	this->x = x;
	this->y = y;
	this->modelMatrix = glm::mat3(1);
	//this->modelMatrix *= transform2D::Scale(scale, scale);
	this->modelMatrix *= transform2D::Translate(x, y);

	this->modelMatrix *= transform2D::Translate(ENEMY_BODY_LENGTH/2,  ENEMY_BODY_LENGTH/2);
	this->modelMatrix *= transform2D::Rotate(rotationAngle);
	this->modelMatrix *= transform2D::Translate(-ENEMY_BODY_LENGTH / 2, -ENEMY_BODY_LENGTH / 2);

	
	this->eye1.setModelMatrix(modelMatrix * transform2D::Translate(ENEMY_BODY_LENGTH*0.7, - ENEMY_BODY_LENGTH*0.1));
	this->eye2.setModelMatrix(modelMatrix * transform2D::Translate(0.7 * ENEMY_BODY_LENGTH, ENEMY_BODY_LENGTH * 0.7));

};
glm::mat3 Enemy::getModelMatrix()
{
	return this->modelMatrix;
}
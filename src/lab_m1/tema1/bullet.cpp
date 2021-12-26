#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>
#include "bullet.h"

using namespace std;
Bullet::Bullet(float initialX, float initialY, float shootAngle)
{
	this->remainingDistance = 800;
	this->shootAngle = shootAngle;
	this->currentX = initialX;
	this->currentY = initialY;
}

Bullet::~Bullet() 
{

}

float Bullet::getCurrentX()
{
	return currentX;
}

float Bullet::getCurrentY()
{
	return currentY;
}

float Bullet::getShootAngle()
{
	return shootAngle;
}

float Bullet::getRemainingDistance()
{
	return this->remainingDistance;
}

void Bullet::setX(float x)
{
	this->currentX = x;
}

void Bullet::setY(float y) {
	this->currentY = y;
}

void Bullet::setRemainingDistance(float remainingDistance)
{
	this->remainingDistance = remainingDistance;
}
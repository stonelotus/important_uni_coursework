#include "lab_m1/tema2/entities/bullet/bullet.h"

Bullet::Bullet(){}
Bullet::Bullet(dimensionsTriplet position, float radius):Sphere(position,radius) {
	this->remaining_distance = BULLETS_LIFE_DISTANCE;
}
Bullet::~Bullet() {};
float Bullet::getRemainingDistance() {
	return this->remaining_distance;
}
void Bullet::ModifyRemainingDistance(float offset) {
	this->remaining_distance += offset;
}

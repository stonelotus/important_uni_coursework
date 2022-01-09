#include "enemy.h"

Enemy::Enemy(){}
Enemy::Enemy(dimensionsTriplet position, dimensionsTriplet size):x(position.x), y(position.y), z(position.z),
																 sizeX(position.x), sizeY(size.y), sizeZ(size.z) {
	body = Sphere(position, ENEMY_SIZE);
	left_eye = Sphere({ position.x + ENEMY_SIZE * 0.42f,position.y + ENEMY_SIZE*0.2f,position.z - ENEMY_SIZE*0.3f }, ENEMY_SIZE/5);
	right_eye = Sphere({ position.x + ENEMY_SIZE * 0.42f,position.y + ENEMY_SIZE*0.2f,position.z + ENEMY_SIZE*0.3f }, ENEMY_SIZE/5);
	this->current_direction = 0;
};
Enemy::~Enemy() {};

void Enemy::Move(dimensionsTriplet offset) {
	this->x += offset.x;
	this->y += offset.y;
	this->z += offset.z;

	body.Move(offset);
	left_eye.Move(offset);
	right_eye.Move(offset);
}

int Enemy::getDirection() {
	return this->current_direction;
}
void Enemy::setDirection(int direction) {
	//std::cout << "Chosen direction: " << direction << std::endl;
	switch (direction) {
		case 1: this->current_direction = 1; break;
		case 2: this->current_direction = 2; break;
		case 3: this->current_direction = 3; break;
		default: this->current_direction = 0; break;
	}
}
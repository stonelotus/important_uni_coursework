#include "player.h"
#include <iostream>
using namespace std;

Player::Player(float x, float y, float z):x(x),y(y),z(z){

	body = Box({ x = this->x + 0.f,
				 y = this->y + 1.2f,
		         z = this->z + 0.f },				// position
			   { x = 0.7f, y = 0.7f, z = 0.7f });	// sizes
	head = Box({ x = body.getPosition().x + 0.f,
				 y = body.getPosition().y + 0.6f,
				 z = body.getPosition().z + 0.f },
			   { x = 0.4f,y = 0.4f, z = 0.4f });
	left_leg = Box({ body.getPosition().x - 0.3f,
					 body.getPosition().y - 0.8f,
					 body.getPosition().z - 0.f },			//position
					 { 0.2f,0.7f,0.2f });			//sizes
	right_leg = Box({ body.getPosition().x + 0.3f,
					  body.getPosition().y - 0.8f,
					  body.getPosition().z - 0.f },			//position
					  { 0.2f,0.7f,0.2f });			//sizes
	right_arm = Box({ body.getPosition().x + 0.6f,
					  body.getPosition().y + 0.f,
					  body.getPosition().z - 0.f },			//position
					  { 0.2f,0.7f,0.2f });			//sizes
	left_arm = Box({ body.getPosition().x - 0.6f,
					  body.getPosition().y + 0.f,
					  body.getPosition().z - 0.f },			//position
					  { 0.2f,0.7f,0.2f });			//sizes
	left_palm = Box({ body.getPosition().x - 0.6f,
					  body.getPosition().y - 0.45f,
					  body.getPosition().z - 0.f },			//position
					{ 0.2f,0.2,0.2f });				//sizes
	right_palm = Box({ body.getPosition().x + 0.6f,
					  body.getPosition().y - 0.45f,
					  body.getPosition().z - 0.f },			//position
				     { 0.2f,0.2f,0.2f });			//sizes

	this->health = 100;
}

Player::Player(){}
Player::~Player() {};

void Player::Move(float x, float y, float z){
	this->last_position = { x,y,z };
	this->x += x;
	this->y += y;
	this->z += z;
	
	body.Move(x, y, z);
	head.Move(x, y, z);
	left_leg.Move(x, y, z);
	right_leg.Move(x, y, z);
	right_arm.Move(x, y, z);
	left_arm.Move(x, y, z);
	right_palm.Move(x, y, z);
	left_palm.Move(x, y, z);

	//for (int i = 0; i < 6; i++) {
	//	player_parts[i].setPosition(x, y, z);
	//}
}

void Player::setPosition(dimensionsTriplet position) {
	this->x = position.x;
	this->y = position.y;
	this->z = position.z;

	body.setPosition(x, y, z);
	head.setPosition(x, y, z);
	left_leg.setPosition(x, y, z);
	right_leg.setPosition(x, y, z);
	right_arm.setPosition(x, y, z);
	left_arm.setPosition(x, y, z);
	right_palm.setPosition(x, y, z);
	left_palm.setPosition(x, y, z);

}
void Player::setLastPosition() {
	this->last_position = { this->x,this->y,this->z };
}
dimensionsTriplet Player::getLastMove	() {
	return this->last_position;
}
dimensionsTriplet Player::getPosition() {
	return { x = this->x,y = this->y,z = this->z };
}
float Player::getHealth() {
	return this->health;
}
void Player::modifyHealth(float offset) {
	this->health += offset;
}
void Player::setAngle(float angle) {
	this->angle = angle;

	body.setAngle(angle);
	head.setAngle(angle);
	left_leg.setAngle(angle);
	right_leg.setAngle(angle);
	right_arm.setAngle(angle);
	left_arm.setAngle(angle);
	right_palm.setAngle(angle);
	left_palm.setAngle(angle);
}
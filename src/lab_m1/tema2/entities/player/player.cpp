#include "player.h"


Player::Player(float x, float y, float z):x(x),y(y),z(z){
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix *= transform3D::Translate(this->x, this->y, this->z);
	body = Box({ x = this->x + 0.f,
				 y = this->y + 1.2f,
		         z = this->z + 0.f },				// position
			   { x = 0.7f, y = 0.7f, z = 0.7f });	// sizes
	head = Box({ x = body.getX() + 0.f,
				 y = body.getY() + 0.6f,
				 z = body.getZ() + 0.f },
			   { x = 0.4f,y = 0.4f, z = 0.4f });
	left_leg = Box({ body.getX() - 0.3f,
					 body.getY() - 0.8f,
					 body.getZ() - 0.f },			//position
					 { 0.2f,0.7f,0.2f });			//sizes
	right_leg = Box({ body.getX() + 0.3f,
					  body.getY() - 0.8f,
					  body.getZ() - 0.f },			//position
					  { 0.2f,0.7f,0.2f });			//sizes

}

Player::Player(){}
Player::~Player() {};

glm::mat4 Player::getModelMatrix() {
	return this->modelMatrix;
}



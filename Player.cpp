#include "Player.h"


Player::Player(glm::vec3 position):position(position),hitBox(position){
    isJumping = false;
    isFalling = false;
    jumpForce = 0.5f;
    velocity = glm::vec3(0.0f);
    hitBox = BoundingBox(position);
}


void Player::jump() {
    if (!isJumping && !isFalling) {
        velocity.z += jumpForce;
        isJumping = true;
    }
}

void Player::overwritePosition(glm::vec3 new_position) {
    position = new_position;
    hitBox.updatePosition(new_position, 1);
}
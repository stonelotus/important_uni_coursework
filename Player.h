#include <glm/glm.hpp>
#include "BoundingBox.h"
class Player {
public:
    Player(glm::vec3 position);
    glm::vec3 position; // The player's current position
    glm::vec3 velocity; // The player's current velocity

    bool isJumping;
    bool isFalling;

    // The player's bounding box for collision detection
    BoundingBox hitBox;

    float jumpForce; // Force applied when the player jumps
    
    
    void jump();
    void overwritePosition(glm::vec3 new_position);

};


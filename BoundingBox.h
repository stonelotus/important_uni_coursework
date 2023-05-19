#pragma once
#include <glm/glm.hpp>

class BoundingBox {
public:
    BoundingBox() {
        min = glm::vec3(0.0f);
		max = glm::vec3(0.0f);
    };
    BoundingBox(glm::vec3 owner_position) {
        updatePosition(owner_position,1);
    }

    BoundingBox(glm::vec3 owner_position, int length) {
        updatePosition(owner_position, length);
    }

    bool intersects(const BoundingBox& other) const {
        if (max.x < other.min.x || min.x > other.max.x) return false;
        if (max.y < other.min.y || min.y > other.max.y) return false;
        if (max.z < other.min.z || min.z > other.max.z) return false;
        return true;
    }

    void updatePosition(glm::vec3 new_position, int length) {
		min = new_position;
		max = glm::vec3(new_position) + glm::vec3(length);
	}
    glm::vec3 getMin() const {
		return min;
	}
    glm::vec3 getMax() const {
		return max;
	}
 //   glm::vec2 getMargins() const {
	//	return glm::vec2(min,max);
	//}

private:
    glm::vec3 min; 
    glm::vec3 max; 

};

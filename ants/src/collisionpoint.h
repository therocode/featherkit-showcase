#pragma once
#include <glm/glm.hpp>

struct CollisionPoint
{
    CollisionPoint();
    CollisionPoint(glm::vec2 position);
    glm::vec2 point;    // relative to origin
    bool falling;
};

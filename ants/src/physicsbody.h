#pragma once
#include "collisionpoint.h"
#include <glm/glm.hpp>

class PhysicsBody
{
    public:
        PhysicsBody();

        // equivalent to middle of sprite
        glm::vec2 origin;

        // relative to origin
        CollisionPoint frontGroundPoint;
        CollisionPoint backGroundPoint;

        float angle;    // radians

        float baseVelocity;
        glm::vec2 actualVelocity;
        glm::vec2 fallingVelocity;

        glm::vec2 frontGroundPointInWorldSpace();
        glm::vec2 backGroundPointInWorldSpace();
};

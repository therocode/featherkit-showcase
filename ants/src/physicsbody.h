#pragma once
#include "collisionpoint.h"
#include <glm/glm.hpp>

class PhysicsBody
{
    public:
        PhysicsBody() = default;
        PhysicsBody(glm::vec2 pos);

        glm::vec2 getFGPInWorldSpace();
        glm::vec2 getBGPInWorldSpace();

        CollisionPoint getFGP();
        CollisionPoint getBGP();

        void setFGPAsFalling(bool falling);
        void setBGPAsFalling(bool falling);

        glm::vec2 getPosition();
        void setPosition(glm::vec2 pos);

        float getAngle();
        void setAngle(float a);

        glm::vec2 recalculateVelocity();
        glm::vec2 getActualVelocity();
        glm::vec2 getFallingVelocity();
        void setActualVelocity(glm::vec2 vel);
        void setFallingVelocity(glm::vec2 vel);

    private:
        // equivalent to middle of sprite
        glm::vec2 position;

        // relative to position
        CollisionPoint frontGroundPoint;
        CollisionPoint backGroundPoint;

        // in radians, starting from (1, 0)
        float angle;    

        float baseVelocity;
        glm::vec2 actualVelocity;
        glm::vec2 fallingVelocity;
};

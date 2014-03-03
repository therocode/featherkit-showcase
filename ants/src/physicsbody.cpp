#include "physicsbody.h"
#include <iostream>
//#include <math.h>

PhysicsBody::PhysicsBody(glm::vec2 pos, bool goingRight)
{
    fallingVelocity = glm::vec2(0.0f, 0.0f);
    angle = 0.0f;   // radians
    position = pos;

    if(goingRight)
    {
        baseVelocity = 1.0f;
        frontGroundPoint = CollisionPoint({15.0f, 8.0f});
        backGroundPoint = CollisionPoint({-15.0f, 8.0f});
    }
    else
    {
        baseVelocity = -1.0f;
        frontGroundPoint = CollisionPoint({15.0f, 8.0f});
        backGroundPoint = CollisionPoint({-15.0f, 8.0f});
    }

}

glm::vec2 PhysicsBody::getFGPInWorldSpace()
{
    glm::vec2 point;
    point = glm::mat2x2(cos(angle), -sin(angle), sin(angle), cos(angle)) * frontGroundPoint.point;
    point += position;
    return point;
}

glm::vec2 PhysicsBody::getBGPInWorldSpace()
{
    glm::vec2 point;
    point = glm::mat2x2(cos(angle), -sin(angle), sin(angle), cos(angle)) * backGroundPoint.point;
    point += position;
    return point;
}

CollisionPoint PhysicsBody::getFGP()
{
    return frontGroundPoint;
}

CollisionPoint PhysicsBody::getBGP()
{
    return backGroundPoint;
}

void PhysicsBody::setFGPAsFalling(bool falling)
{
    frontGroundPoint.falling = falling;
}

void PhysicsBody::setBGPAsFalling(bool falling)
{
    backGroundPoint.falling = falling;
}

glm::vec2 PhysicsBody::getPosition()
{
    return position;
}

void PhysicsBody::setPosition(glm::vec2 pos)
{
    position = pos;
}

float PhysicsBody::getAngle()
{
    return angle;
}

void PhysicsBody::setAngle(float a)
{
    angle = a;
}

glm::vec2 PhysicsBody::recalculateVelocity()
{
    return glm::vec2(baseVelocity * cos(angle), baseVelocity * sin(angle));
}

glm::vec2 PhysicsBody::getActualVelocity()
{
    return actualVelocity;
}

glm::vec2 PhysicsBody::getFallingVelocity()
{
    return fallingVelocity;
}

void PhysicsBody::setActualVelocity(glm::vec2 vel)
{
    actualVelocity = vel;
}

void PhysicsBody::setFallingVelocity(glm::vec2 vel)
{
    fallingVelocity = vel;
}

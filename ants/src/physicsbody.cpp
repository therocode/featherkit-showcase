#include "physicsbody.h"
#include <iostream>
//#include <math.h>

PhysicsBody::PhysicsBody(glm::vec2 pos)
{
    baseVelocity = -2.0f;
    fallingVelocity = glm::vec2(0.0f, 0.0f);
    angle = 0.0f;
    position = pos;
    std::cout << "position is: " << position.x << " and " << position.y << "\n";
    frontGroundPoint = CollisionPoint({40.0f, 25.0f});
    backGroundPoint = CollisionPoint({-40.0f, 25.0f});
}

glm::vec2 PhysicsBody::frontGroundPointInWorldSpace()
{
    glm::vec2 point;
    point = glm::mat2x2(cos(angle), -sin(angle), sin(angle), cos(angle)) * frontGroundPoint.point;
    point += position;
    return point;
}

glm::vec2 PhysicsBody::backGroundPointInWorldSpace()
{
    glm::vec2 point;
    point = glm::mat2x2(cos(angle), -sin(angle), sin(angle), cos(angle)) * backGroundPoint.point;
    point += position;
    return point;
}

CollisionPoint& PhysicsBody::getFGP()
{
    return frontGroundPoint;
}

CollisionPoint& PhysicsBody::getBGP()
{
    return backGroundPoint;
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

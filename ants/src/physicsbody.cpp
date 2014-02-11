#include "physicsbody.h"
//#include <math.h>

PhysicsBody::PhysicsBody()
{
    baseVelocity = 5.0f;
    //actualVelocity = glm::vec2(baseVelocity * cos(angle), baseVelocity * sin(angle));  
    fallingVelocity = glm::vec2(0.0f, 0.0f);
    origin = glm::vec2(800.0f, 600.0f);
    frontGroundPoint = CollisionPoint({50.0f, 50.0f});
    backGroundPoint = CollisionPoint({0.0f, 20.0f});
}

glm::vec2 PhysicsBody::frontGroundPointInWorldSpace()
{
    glm::vec2 point;
    point = glm::mat2x2(cos(angle), -sin(angle), sin(angle), cos(angle)) * frontGroundPoint.point;
    point += origin;
    return point;
}

glm::vec2 PhysicsBody::backGroundPointInWorldSpace()
{
    glm::vec2 point;
    point = glm::mat2x2(cos(angle), -sin(angle), sin(angle), cos(angle)) * backGroundPoint.point;
    point += origin;
    return point;
}

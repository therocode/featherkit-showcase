#include "physics.h"
#include <math.h>
#include <iostream>

const float degree = 0.0174532925f;    

Physics::Physics(fea::MessageBus& bus)
    :   messageBus(bus)
{
    ant = PhysicsBody({800.0f, 600.0f});
    dirtTexture = nullptr;
    gravity = glm::vec2(0.0f, 1.0f);
    thresholdAngle = 3.14f/2.0f;
}

Physics::~Physics()
{
}

void Physics::setTexture(fea::Texture* texture)
{
    dirtTexture = texture;
}

void Physics::update()
{
    addVelocity(ant);
    addFalling(ant);
    terrainCheck(ant);

    messageBus.send(AntPositionMessage(ant.getPosition(), ant.getAngle()));
    messageBus.send(AntPointsMessage(ant.getFGPInWorldSpace(), ant.getBGPInWorldSpace()));
}

void Physics::addVelocity(PhysicsBody& body)
{
    if(body.getFGP().falling && body.getBGP().falling)
    {
        body.setPosition(body.getPosition() + body.getFallingVelocity());
    }
    else if(!(body.getFGP().falling || body.getBGP().falling))
    {
        glm::vec2 velocity = body.recalculateVelocity();
        body.setPosition(body.getPosition() + velocity);
    }
}

void Physics::addFalling(PhysicsBody& body)
{
    if(body.getFGP().falling && body.getBGP().falling)
    {
        body.setFallingVelocity(body.getFallingVelocity() + gravity);
    }
    else if(body.getFGP().falling)
    {
        body.setPosition(rotatePoint(body.getPosition(), -degree, body.getBGPInWorldSpace()));
        body.setAngle(body.getAngle() - degree);    // rotate the ant
    }
    else if(body.getBGP().falling)
    {
        body.setPosition(rotatePoint(body.getPosition(), degree, body.getFGPInWorldSpace()));
        body.setAngle(body.getAngle() + degree);    // rotate the ant
    }
}

void Physics::terrainCheck(PhysicsBody& body)
{
    // Front Point check
    while(terrainCollisionAt(body.getFGPInWorldSpace()))
    {
        body.setPosition(rotatePoint(body.getPosition(), degree, body.getBGPInWorldSpace()));
        body.setAngle(body.getAngle() + degree);    
        // check for critical/threshold angle
    }
    bool frontColliding = terrainCollisionAt(body.getFGPInWorldSpace() + glm::vec2(0.0f, 5.0f));
    ant.setFGPAsFalling(!frontColliding);   // falls if air below, otherwise not falling

    // Back Point check
    while(terrainCollisionAt(body.getBGPInWorldSpace()))
    {
        body.setPosition(rotatePoint(body.getPosition(), -degree, body.getFGPInWorldSpace()));
        body.setAngle(body.getAngle() - degree);
        // check for critical/threshold angle
    }
    bool backColliding = terrainCollisionAt(body.getBGPInWorldSpace() + glm::vec2(0.0f, 5.0f));
    ant.setBGPAsFalling(!backColliding);
}

bool Physics::terrainCollisionAt(glm::vec2 pos)
{
    glm::uvec2 position = (glm::uvec2)(pos / 2.0f);
    return dirtTexture->getPixel(position.x, position.y).a() != 0.0f;
}

glm::vec2 Physics::rotatePoint(glm::vec2 pointToRotate, float degreesToRotate, glm::vec2 pointOfOrigin)
{
    pointToRotate = pointToRotate - pointOfOrigin;    // get pointToRotate's position relative to the pointOfOrigin
    pointToRotate = glm::mat2x2(cos(degreesToRotate), -sin(degreesToRotate), sin(degreesToRotate), cos(degreesToRotate)) * pointToRotate;   // rotate point around the pointOfOrigin
    return pointToRotate + pointOfOrigin;
}

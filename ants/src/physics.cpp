#include "physics.h"
#include <math.h>
#include <iostream>

const float degree = 0.0174532925f;    

Physics::Physics(fea::MessageBus& bus)
    :   messageBus(bus)
{
    messageBus.addSubscriber<DirtTextureSetMessage>(*this);
    messageBus.addSubscriber<AntCreationMessage>(*this);

    dirtTexture = nullptr;
    gravity = glm::vec2(0.0f, 1.0f);
    //thresholdAngle = 3.14f/2.0f;
}

Physics::~Physics()
{
    messageBus.removeSubscriber<DirtTextureSetMessage>(*this);
    messageBus.removeSubscriber<AntCreationMessage>(*this);
}

void Physics::handleMessage(const DirtTextureSetMessage& mess)
{
    std::tie(dirtTexture) = mess.mData;
}

void Physics::handleMessage(const AntCreationMessage& mess)
{
    bool digging;
    glm::vec2 position;
    std::tie(digging, position) = mess.mData;

    PhysicsBody ant(position);

    ants.push_back(ant);
}

void Physics::update()
{
    for(int i = 0; i < ants.size(); i++)
    {
        PhysicsBody& ant = ants.at(i);
        addVelocity(ant);
        addFalling(ant);
        terrainCheck(ant);

        messageBus.send(AntPositionMessage(i, ant.getPosition(), ant.getAngle()));
        messageBus.send(AntPointsMessage(ant.getFGPInWorldSpace(), ant.getBGPInWorldSpace()));
    }
}

// private //   //////////////////////////////////////////////////////////////////////////////////////////////

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
    body.setFGPAsFalling(!frontColliding);   // falls if air below, otherwise not falling

    // Back Point check
    while(terrainCollisionAt(body.getBGPInWorldSpace()))
    {
        body.setPosition(rotatePoint(body.getPosition(), -degree, body.getFGPInWorldSpace()));
        body.setAngle(body.getAngle() - degree);
        // check for critical/threshold angle
    }
    bool backColliding = terrainCollisionAt(body.getBGPInWorldSpace() + glm::vec2(0.0f, 5.0f));
    body.setBGPAsFalling(!backColliding);
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

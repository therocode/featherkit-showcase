#include "physics.h"
#include <math.h>
#include <iostream>

const float degree = 0.0174532925f;    

Physics::Physics(fea::MessageBus& bus)
    :   messageBus(bus)
{
    messageBus.addSubscriber<DirtTextureSetMessage>(*this);
    messageBus.addSubscriber<AntCreationMessage>(*this);
    messageBus.addSubscriber<AntDeletionMessage>(*this);
    messageBus.addSubscriber<AntStartedDiggingMessage>(*this);
    messageBus.addSubscriber<AntStoppedDiggingMessage>(*this);

    dirtTexture = nullptr;
    gravity = glm::vec2(0.0f, 1.0f);
}

Physics::~Physics()
{
    messageBus.removeSubscriber<DirtTextureSetMessage>(*this);
    messageBus.removeSubscriber<AntCreationMessage>(*this);
    messageBus.removeSubscriber<AntDeletionMessage>(*this);
    messageBus.removeSubscriber<AntStartedDiggingMessage>(*this);
    messageBus.removeSubscriber<AntStoppedDiggingMessage>(*this);
}

void Physics::update()
{
    std::vector<size_t> antsOutsideOfBoundary;

    for(auto i = ants.begin(); i != ants.end(); i++)
    {
        size_t antId = i->first;
        PhysicsBody& ant = i->second;

        updateVelocity(ant, antId);

        // for the non-digging ants only:
        auto iterator = targetPositions.find(antId);
        if(!(iterator != targetPositions.end()))
        {
            updateGravityState(ant);
            terrainCheck(ant);
        }

        messageBus.send(AntPositionMessage(antId, ant.getPosition(), ant.getAngle()));

        // check if any ants are outside of the screen boundaries
        if((ant.getPosition().x < 20.0f) || (ant.getPosition().x > 1576.0f))
        {
            antsOutsideOfBoundary.push_back(antId);
        }
    }

    for(size_t& id : antsOutsideOfBoundary)
    {
        messageBus.send(AntOutsideBoundariesMessage(id));
    }
}

void Physics::handleMessage(const DirtTextureSetMessage& mess)
{
    std::tie(dirtTexture) = mess.mData;
}

void Physics::handleMessage(const AntCreationMessage& mess)
{
    size_t id;
    bool digging;
    bool goingRight;
    glm::vec2 position;
    float velocity;
    std::tie(id, digging, goingRight, position, velocity) = mess.mData;

    PhysicsBody ant(position, goingRight, velocity);
    ants.emplace(id, ant);
}

void Physics::handleMessage(const AntDeletionMessage& mess)
{
    int index;
    std::tie(index) = mess.mData;

    ants.erase(index);
}

void Physics::handleMessage(const AntStartedDiggingMessage& mess)
{
    size_t index;
    glm::vec2 targetPosition;
    std::tie(index, targetPosition) = mess.mData;

    targetPositions.emplace(index, targetPosition);
}

void Physics::handleMessage(const AntStoppedDiggingMessage& mess)
{
    size_t index;
    std::tie(index) = mess.mData;

    targetPositions.erase(index);
}

void Physics::updateVelocity(PhysicsBody& body, size_t id)
{
    // if the digger ant is digging:
    auto iterator = targetPositions.find(id);
    if(iterator != targetPositions.end())
    {
        glm::vec2 targetPosition = targetPositions.at(id);
        glm::vec2 velocity = body.recalculateVelocity(targetPosition);
        body.setPosition(body.getPosition() + velocity);

        float angle = (float)atan(velocity.y/velocity.x);
        body.setAngle(-angle);
    }

    // if any other ant
    else
    {
        // if both points falling, then fall
        if(body.getFGP().falling && body.getBGP().falling)  
        {
            body.setPosition(body.getPosition() + body.getFallingVelocity());
        }
        // if none falling, then walk
        else if(!(body.getFGP().falling || body.getBGP().falling))  
        {
            glm::vec2 velocity = body.recalculateVelocity();
            body.setPosition(body.getPosition() + velocity);
        }
    }
}

void Physics::updateGravityState(PhysicsBody& body)
{
    // if freefalling, apply gravity
    if(body.getFGP().falling && body.getBGP().falling)
    {
        body.setFallingVelocity(body.getFallingVelocity() + gravity);
    }
    else if(body.getFGP().falling)
    {// rotate the ant relative to back point
        body.setFallingVelocity(glm::vec2(0.0f, 0.0f));
        body.setPosition(rotatePoint(body.getPosition(), -degree * 3.0f, body.getBGPInWorldSpace()));
        body.setAngle(body.getAngle() - degree * 3.0f);    
    }
    else if(body.getBGP().falling)
    {// rotate the ant relative to front point
        body.setFallingVelocity(glm::vec2(0.0f, 0.0f));
        body.setPosition(rotatePoint(body.getPosition(), degree * 3.0f, body.getFGPInWorldSpace()));
        body.setAngle(body.getAngle() + degree * 3.0f);    
    }
}

void Physics::terrainCheck(PhysicsBody& body)
{
    // Front Point check
    while(terrainCollisionAt(body.getFGPInWorldSpace()))
    {
        body.setPosition({body.getPosition().x, body.getPosition().y - 1.0f});
    }
    bool frontColliding = terrainCollisionAt(body.getFGPInWorldSpace() + glm::vec2(0.0f, 4.0f));
    body.setFGPAsFalling(!frontColliding);   // falls if air below, otherwise not falling

    // Back Point check
    while(terrainCollisionAt(body.getBGPInWorldSpace()))
    {
        body.setPosition({body.getPosition().x, body.getPosition().y - 1.0f});
    }
    bool backColliding = terrainCollisionAt(body.getBGPInWorldSpace() + glm::vec2(0.0f, 4.0f));
    body.setBGPAsFalling(!backColliding);
}

bool Physics::terrainCollisionAt(glm::vec2 pos)
{
    glm::uvec2 position = (glm::uvec2)(pos/2.0f);
    return dirtTexture->getPixel(position.x, position.y).a() != 0.0f;
}

glm::vec2 Physics::rotatePoint(glm::vec2 pointToRotate, float degreesToRotate, glm::vec2 pointOfOrigin)
{
    // get pointToRotate's position relative to the pointOfOrigin:
    pointToRotate = pointToRotate - pointOfOrigin;    
    // rotate point around the pointOfOrigin:
    pointToRotate = glm::mat2x2(cos(degreesToRotate), -sin(degreesToRotate), sin(degreesToRotate), cos(degreesToRotate)) * pointToRotate;   
    return pointToRotate + pointOfOrigin;
}

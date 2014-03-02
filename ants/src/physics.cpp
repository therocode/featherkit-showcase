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

    dirtTexture = nullptr;
    gravity = glm::vec2(0.0f, 1.0f);
    //thresholdAngle = 3.14f/2.0f;
}

Physics::~Physics()
{
    messageBus.removeSubscriber<DirtTextureSetMessage>(*this);
    messageBus.removeSubscriber<AntCreationMessage>(*this);
    messageBus.removeSubscriber<AntDeletionMessage>(*this);
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
    std::tie(id, digging, goingRight, position) = mess.mData;

    PhysicsBody ant(position, goingRight);

    ants.emplace(id, ant);
}

void Physics::handleMessage(const AntDeletionMessage& mess)
{
    int index;
    std::tie(index) = mess.mData;

    ants.erase(index);
}

void Physics::update()
{
    std::vector<size_t> antsToDelete;
    for(auto i = ants.begin(); i != ants.end();)
    {
        PhysicsBody& ant = i->second;
        addVelocity(ant);
        addFalling(ant);
        terrainCheck(ant);

        std::cout << "BAJS\n";

        messageBus.send(AntPositionMessage(i->first, ant.getPosition(), ant.getAngle()));
        //messageBus.send(AntPointsMessage(ant.getFGPInWorldSpace(), ant.getBGPInWorldSpace()));

        // check if any ants outside of boundary
        if((ant.getPosition().x < 24.0f) || (ant.getPosition().x > 1176.0f))
        {
            std::cout << "hi\n";
            messageBus.send(AntOutsideBoundariesMessage(i->first));
            antsToDelete.push_back(i->first);
        }
    }
    // delete ants here
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
        //std::cout << "--both falling\n";
        body.setFallingVelocity(body.getFallingVelocity() + gravity);
    }
    else if(body.getFGP().falling)
    {
        //std::cout << "--front falling\n";
        body.setFallingVelocity(glm::vec2(0.0f, 0.0f));
        body.setPosition(rotatePoint(body.getPosition(), -degree, body.getBGPInWorldSpace()));
        body.setAngle(body.getAngle() - degree);    // rotate the ant
    }
    else if(body.getBGP().falling)
    {
        //std::cout << "--back falling\n";
        body.setFallingVelocity(glm::vec2(0.0f, 0.0f));
        body.setPosition(rotatePoint(body.getPosition(), degree, body.getFGPInWorldSpace()));
        body.setAngle(body.getAngle() + degree);    // rotate the ant
    }
    else
    {
        //std::cout << "--none falling\n";
    }
}

void Physics::terrainCheck(PhysicsBody& body)
{
    /*
    // Front Point check
    while(terrainCollisionAt(body.getFGPInWorldSpace()))
    {
        body.setPosition(rotatePoint(body.getPosition(), degree, body.getBGPInWorldSpace()));
        body.setAngle(body.getAngle() + degree);    
        // check for critical/threshold angle
        float hej = glm::degrees(body.getAngle());
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
    */
    
    // Front Point check
    float hej = glm::degrees(body.getAngle());
    //std::cout << "start front loop of angle: " << hej << "\n";
    int n= 0;
    while(terrainCollisionAt(body.getFGPInWorldSpace()))
    {
        body.setPosition(body.getPosition() - 1.0f);
        n++;
    }
    std::cout << "counter is: " << n << "\n";
    //std::cout << "END front loop\n";
    bool frontColliding = terrainCollisionAt(body.getFGPInWorldSpace() + glm::vec2(0.0f, 4.0f));
    body.setFGPAsFalling(!frontColliding);   // falls if air below, otherwise not falling

    // Back Point check
    //std::cout << "start back loop of angle: " << hej << "\n";
    while(terrainCollisionAt(body.getBGPInWorldSpace()))
    {
        //std::cout << "BGP at: " << body.getBGPInWorldSpace().x << ", " << body.getBGPInWorldSpace().y << "\n";
        body.setPosition(body.getPosition() - 1.0f);
    }
    //std::cout << "END back loop\n\n";
    bool backColliding = terrainCollisionAt(body.getBGPInWorldSpace() + glm::vec2(0.0f, 4.0f));
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

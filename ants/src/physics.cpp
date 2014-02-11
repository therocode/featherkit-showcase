#include "physics.h"
#include <math.h>
#include <iostream>

Physics::Physics(fea::MessageBus& bus)
    :   messageBus(bus)
{
    dirtTexture = nullptr;
    gravity = 5.0f;
    thresholdAngle = 3.14f/2.0f;
    ant.frontGroundPoint.falling = true;
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
    std::cout << "ant point states are: " << ant.frontGroundPoint.falling << " and " << ant.backGroundPoint.falling << "\n";
    //addVelocity(ant);
    addFalling(ant);
    //terrainCheck(ant);

    messageBus.send(AntPositionMessage(ant.origin, ant.angle));
}

void Physics::addVelocity(PhysicsBody& body)
{
    body.actualVelocity = glm::vec2(body.baseVelocity * cos(body.angle), body.baseVelocity * sin(body.angle));
    body.origin += body.actualVelocity;
    if(body.frontGroundPoint.falling && body.backGroundPoint.falling)
    {
        body.origin += body.fallingVelocity;
    }
}

void Physics::addFalling(PhysicsBody& body)
{
    if(body.frontGroundPoint.falling && body.backGroundPoint.falling)
    {
        body.fallingVelocity.y += gravity;
    }
    else if(body.frontGroundPoint.falling)
    {
        glm::vec2 point = body.backGroundPointInWorldSpace();    
        point = body.origin - point;    // get origin's position relative to the back point
        float degree = 0.0174532925f;    
        body.angle -= degree;    // rotate the ant
        point = glm::mat2x2(cos(degree), -sin(degree), sin(degree), cos(degree)) * point;   // rotate origin around the back point
        body.origin = point;
        //messageBus.send(AntPositionMessage(body.origin, body.angle));
    }
    else if(body.backGroundPoint.falling)
    {
        glm::vec2 point = body.frontGroundPointInWorldSpace();
        point = body.origin - point;    // get origin's position relative to the front point
        float degree = 0.0174532925f;    
        body.angle += degree;    // rotate the ant
        point = glm::mat2x2(cos(degree), -sin(degree), sin(degree), cos(degree)) * point;   // rotate origin around the front point
        body.origin = point;
        //messageBus.send(AntPositionMessage(body.origin, body.angle));
    }
}

void Physics::terrainCheck(PhysicsBody& body)
{
    //glm::vec2 possiblePos = body.origin + body.frontGroundPoint + body.actualVelocity;
    glm::vec2 possiblePos = glm::vec2(0.0f, 0.0f);
    while(terrainCollisionAt(possiblePos))
    {
        body.actualVelocity.y = 0.0f;   // this is wrong! shouldn't be zeroing the actual velocities
        body.angle += 0.0174532925f;
        possiblePos = glm::mat2x2(cos(body.angle), -sin(body.angle), sin(body.angle), cos(body.angle)) * possiblePos;

        // check for critical/threshold angle
    }
}

bool Physics::terrainCollisionAt(glm::vec2 pos)
{
    glm::uvec2 position = (glm::uvec2)(pos / 2.0f);
    return dirtTexture->getPixel(position.x, position.y).a() != 0.0f;
}

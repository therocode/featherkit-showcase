#include "physics.h"
#include <math.h>
#include <iostream>

Physics::Physics(fea::MessageBus& bus)
    :   messageBus(bus)
{
    ant = PhysicsBody({800.0f, 600.0f});
    dirtTexture = nullptr;
    gravity = glm::vec2(0.0f, 1.0f);
    thresholdAngle = 3.14f/2.0f;
    ant.getFGP().falling = true;
    //ant.getBGP().falling = true;
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
    //std::cout << "ant point states are: " << ant.getFGP().falling << " and " << ant.getBGP().falling << "\n";
    addVelocity(ant);
    addFalling(ant);
    //terrainCheck(ant);

    messageBus.send(AntPositionMessage(ant.getPosition(), ant.getAngle()));
}

void Physics::addVelocity(PhysicsBody& body)
{
    glm::vec2 velocity = body.recalculateVelocity();
    body.setPosition(body.getPosition() + velocity);

    if(body.getFGP().falling && body.getBGP().falling)
    {
        body.setPosition(body.getPosition() + body.getFallingVelocity());
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
        glm::vec2 point = body.backGroundPointInWorldSpace();    
        glm::vec2 originalPosition = body.getPosition();
        point = originalPosition - point;    // get origin's position relative to the back point
        float degree = 0.0174532925f;    
        body.setAngle(body.getAngle() - degree);    // rotate the ant
        std::cout << "position1 is: " << point.x << " and " << point.y << "\n";
        point = glm::mat2x2(cos(-degree), -sin(-degree), sin(-degree), cos(-degree)) * point;   // rotate origin around the back point
        std::cout << "position2 is: " << point.x << " and " << point.y << "\n";
        body.setPosition(point + body.backGroundPointInWorldSpace());
        //messageBus.send(AntPositionMessage(body.origin, body.angle));
    }
    else if(body.getBGP().falling)
    {
        glm::vec2 point = body.frontGroundPointInWorldSpace();
        point = body.getPosition() - point;    // get origin's position relative to the front point
        float degree = 0.0174532925f;    
        body.setAngle(body.getAngle() + degree);    // rotate the ant
        point = glm::mat2x2(cos(degree), -sin(degree), sin(degree), cos(degree)) * point;   // rotate origin around the front point
        body.setPosition(point);
        //messageBus.send(AntPositionMessage(body.origin, body.angle));
    }
}

void Physics::terrainCheck(PhysicsBody& body)
{
    //glm::vec2 possiblePos = body.origin + body.frontGroundPoint + body.actualVelocity;
    glm::vec2 possiblePos = glm::vec2(0.0f, 0.0f);
    while(terrainCollisionAt(possiblePos))
    {
        body.setActualVelocity(glm::vec2(body.getActualVelocity().x, 0.0f));
        float degree = 0.0174532925f;    
        body.setAngle(body.getAngle() + degree);    // rotate the ant
        //possiblePos = glm::mat2x2(cos(body.angle), -sin(body.angle), sin(body.angle), cos(body.angle)) * possiblePos;

        // check for critical/threshold angle
    }
}

bool Physics::terrainCollisionAt(glm::vec2 pos)
{
    glm::uvec2 position = (glm::uvec2)(pos / 2.0f);
    return dirtTexture->getPixel(position.x, position.y).a() != 0.0f;
}

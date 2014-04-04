#include "digging.h"

Digging::Digging(fea::MessageBus& bus)
    :   messageBus(bus)
{
    digging = false;

    messageBus.addSubscriber<AntPositionMessage>(*this);
    messageBus.addSubscriber<DiggerAntCreatedMessage>(*this);
    messageBus.addSubscriber<DirtTextureSetMessage>(*this);

    dirtTexture = nullptr;

    diggingPositionStartA = glm::vec2(568.0f, 900.0f);
    diggingPositionEndA = glm::vec2(745.0f, 926.0f);
    diggingPositionStartB = glm::vec2(968.0f, 925.0f);
    diggingPositionEndB = glm::vec2(1122.0f, 973.0f);
    radius = 10.0f;

    diggingPosition = DIG_OFF;
}

Digging::~Digging()
{
    messageBus.removeSubscriber<AntPositionMessage>(*this);
    messageBus.removeSubscriber<DiggerAntCreatedMessage>(*this);
    messageBus.removeSubscriber<DirtTextureSetMessage>(*this);
}

void Digging::update()
{
    if(!digging)
    {
        if(glm::length(antPosition - diggingPositionStartA) < radius)
        {
            std::cout << "YEYEYEY\n";
            messageBus.send(AntStartedDiggingMessage(diggerAntId, diggingPositionEndA));     // send to renderer to change animation? 
            digging = true;
            diggingPosition = DIG_A;
        }
        else if(glm::length(antPosition - diggingPositionStartB) < radius)
        {
            messageBus.send(AntStartedDiggingMessage(diggerAntId, diggingPositionEndB));
            digging = true;
            diggingPosition = DIG_B;
        }
    }
    else // digging
    {
        // check it's not at an end point
        if((glm::length(antPosition - diggingPositionEndA) < radius) || (glm::length(antPosition - diggingPositionEndB) < radius))
        {
            std::cout << "hello, I am at the end position :)\n";
            digging = false;
            diggingPosition = DIG_OFF;
            messageBus.send(AntStoppedDiggingMessage(diggerAntId));
        }
        // dig!
        else
        {
            // dig away the texture here
            for(int i = 0; i < 10; i++)
            {
                for(int j = 0; j < 10; j++)
                {
                    dirtTexture->setPixel(antPosition.x/2 + i, antPosition.y/2 + j, fea::Color(255, 255, 255, 0));
                }
            }
            dirtTexture->update();
        }
    }
}

void Digging::handleMessage(const AntPositionMessage& mess)
{
    size_t index;
    glm::vec2 position;
    std::tie(index, position, std::ignore) = mess.mData;
    
    if(index == diggerAntId)
    {
        antPosition = position;
    }
}

void Digging::handleMessage(const DiggerAntCreatedMessage& mess)
{
    size_t id;
    std::tie(id) = mess.mData;

    diggerAntId = id;
}

void Digging::handleMessage(const DirtTextureSetMessage& mess)
{
    std::tie(dirtTexture) = mess.mData;
}

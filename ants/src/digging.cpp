#include "digging.h"

Digging::Digging(fea::MessageBus& bus)
    :   messageBus(bus)
{
    digging = false;

    messageBus.addSubscriber<AntPositionMessage>(*this);
    messageBus.addSubscriber<DiggerAntCreatedMessage>(*this);

    diggingPositionStartA = glm::vec2(568.0f, 900.0f);
    diggingPositionEndA = glm::vec2(959.0f, 1067.0f);
    diggingPositionStartB = glm::vec2(751.0f, 1047.0f);
    diggingPositionEndB = glm::vec2(1122.0f, 1105.0f);
    radius = 10.0f;

    diggingPosition = DIG_OFF;
}

Digging::~Digging()
{
    messageBus.removeSubscriber<AntPositionMessage>(*this);
    messageBus.removeSubscriber<DiggerAntCreatedMessage>(*this);
}

void Digging::update()
{
    if(!digging)
    {
        //std::cout << "ant pos is: " << antPosition.x << ", " << antPosition.y << "\n";
        //std::cout << "dig pos is: " << diggingPositionStartA.x << ", " << diggingPositionStartA.y << "\n\n";

        if(glm::length(antPosition - diggingPositionStartA) < radius)
        {
            std::cout << "YEYEYEY\n";
            messageBus.send(AntStartedDiggingMessage(true));
            digging = true;
        }
        else if(glm::length(antPosition - diggingPositionStartB) < radius)
        {
            messageBus.send(AntStartedDiggingMessage(false));
            digging = true;
        }
    }
    else
    {
        if((glm::length(antPosition - diggingPositionEndA) < radius) || (glm::length(antPosition - diggingPositionEndB) < radius))
        {
            digging = false;
        }
        else
        {
            // make ant vel towards B
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

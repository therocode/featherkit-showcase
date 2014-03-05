#include "digging.h"

Digging::Digging(fea::MessageBus& bus)
    :   messageBus(bus)
{
    digging = false;

    messageBus.addSubscriber<AntPositionMessage>(*this);
    messageBus.addSubscriber<DiggerAntCreatedMessage>(*this);
    /*
    diggingPositionStartA;
    diggingPositionEndA;
    diggingPositionStartB;
    diggingPositionEndB;
    */
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

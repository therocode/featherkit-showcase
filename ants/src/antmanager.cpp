#include "antmanager.h"
#include <stdlib.h>

AntManager::AntManager(fea::MessageBus& bus)
    :   messageBus(bus)
{
    nextAntId = 0;

    spawnPositionA = glm::vec2(20.0f, 1100.0f);
    spawnPositionB = glm::vec2(1575.0f, 925.0f);

    messageBus.addSubscriber<AntOutsideBoundariesMessage>(*this);
}

AntManager::~AntManager()
{
    messageBus.removeSubscriber<AntOutsideBoundariesMessage>(*this);
}

void AntManager::update()
{
    spawnAnts();
}

size_t AntManager::createAnt(bool type, bool goingRight, glm::vec2 position, float velocity)
{
    size_t createdId = nextAntId;
    nextAntId++;

    ants.emplace(createdId, Ant(type));
    messageBus.send(AntCreationMessage(createdId, type, goingRight, position, velocity));
    return createdId;
}

void AntManager::handleMessage(const AntOutsideBoundariesMessage& mess)
{
    int index;
    std::tie(index) = mess.mData;
    ants.erase(index);
    messageBus.send(AntDeletionMessage(index));
}

void AntManager::spawnAnts()
{
    if(rand() % 100 < 2)
    {
        createAnt((rand() % 2), true, spawnPositionA, ((float)(rand() % 25 + 10))/10.0f);
    }

    if(rand() % 100 < 2)
    {
        createAnt((rand() % 2), false, spawnPositionB, ((float)(rand() % 25 + 10))/10.0f);
    }
}

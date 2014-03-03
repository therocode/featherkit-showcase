#include "antmanager.h"
#include <stdlib.h>

AntManager::AntManager(fea::MessageBus& bus)
    :   messageBus(bus)
{
    nextAntId = 0;

    spawnPositionA = glm::vec2(400.0f, 400.0f);
    spawnPositionB = glm::vec2(400.0f, 400.0f);

    messageBus.addSubscriber<AntOutsideBoundariesMessage>(*this);
}

AntManager::~AntManager()
{
    messageBus.removeSubscriber<AntOutsideBoundariesMessage>(*this);
}

void AntManager::update()
{
    spawn();
}

void AntManager::createAnt(bool type, bool goingRight, glm::vec2 position)
{
    ants.emplace(nextAntId, Ant(type));
    messageBus.send(AntCreationMessage(nextAntId, type, goingRight, position));
    nextAntId++;
}

void AntManager::handleMessage(const AntOutsideBoundariesMessage& mess)
{
    int index;
    std::tie(index) = mess.mData;
    ants.erase(index);
    messageBus.send(AntDeletionMessage(index));
}

void AntManager::spawn()
{
    if(rand() % 100 < 5)
    {
        createAnt((rand() % 2), false, spawnPositionA);
    }

    if(rand() % 100 < 5)
    {
        createAnt((rand() % 2), true, spawnPositionB);
    }
}

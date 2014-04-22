#include "antmanager.h"
#include <stdlib.h>

AntManager::AntManager(fea::MessageBus& bus)
    :   messageBus(bus)
{
    nextAntId = 0;

    spawnPositionTop = glm::vec2(1550.0f, 925.0f);
    spawnPositionBottom = glm::vec2(20.0f, 1100.0f);

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

size_t AntManager::createAnt(AntType type, bool goingRight, glm::vec2 position, float velocity)
{
    size_t createdId = nextAntId;
    nextAntId++;

    messageBus.send(AntCreationMessage(createdId, type, goingRight, position, velocity));
    return createdId;
}

void AntManager::handleMessage(const AntOutsideBoundariesMessage& mess)
{
    int index;
    std::tie(index) = mess.mData;
    messageBus.send(AntDeletionMessage(index));
}

void AntManager::spawnAnts()
{
    if(rand() % 1000 < 15)
    {
        createAnt(AntType::NORMAL, false, spawnPositionTop, ((float)(rand() % 25 + 10))/10.0f);
    }

    if(rand() % 1000 < 10)
    {
        createAnt(randomCrystalAntType(), true, spawnPositionBottom, ((float)(rand() % 25 + 10))/10.0f);
    }
}

AntType AntManager::randomCrystalAntType()
{
    int kalle = rand() % 3;
    AntType hej;

    if(kalle == 0)
        hej = AntType::RED;
    else if(kalle == 1)
        hej = AntType::GREEN;
    else
        hej = AntType::BLUE;

    return hej;
}

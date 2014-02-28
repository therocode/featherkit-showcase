#include "antmanager.h"

AntManager::AntManager(fea::MessageBus& bus)
    :   messageBus(bus)
{
}

void AntManager::update()
{
    // spawn new ants

}

void AntManager::createAnt(bool type, bool goingRight, glm::vec2 position)
{
    ants.push_back(Ant(type));
    messageBus.send(AntCreationMessage(type, goingRight, position));
}

void AntManager::handleMessage(const AntOutsideBoundariesMessage& mess)
{
    int index;
    std::tie(index) = mess.mData;
    ants.erase(ants.begin() + index);
    messageBus.send(AntDeletionMessage(index));
}

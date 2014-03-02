#include "antmanager.h"

AntManager::AntManager(fea::MessageBus& bus)
    :   messageBus(bus)
{
    nextAntId = 0;
}

void AntManager::update()
{
    // spawn new ants

}

void AntManager::createAnt(bool type, bool goingRight, glm::vec2 position)
{
    ants.emplace(nextAntId, Ant(type));
    messageBus.send(AntCreationMessage(nextAntId, type, goingRight, position));
    nextAntId++;
}

void AntManager::handleMessage(const AntOutsideBoundariesMessage& mess)
{
    std::cout << "hello\n";
    /*
    int index;
    std::tie(index) = mess.mData;
    ants.erase(ants.begin() + index);
    messageBus.send(AntDeletionMessage(index));
    */
}

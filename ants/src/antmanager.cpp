#include "antmanager.h"

AntManager::AntManager(fea::MessageBus& bus)
    :   messageBus(bus)
{
}

void AntManager::createAnt(bool type, bool goingRight, glm::vec2 position)
{
    ants.push_back(Ant(type));
    messageBus.send(AntCreationMessage(type, goingRight, position));
}

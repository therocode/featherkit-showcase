#include "antmanager.h"

AntManager::AntManager(fea::MessageBus& bus)
    :   messageBus(bus)
{
}

void AntManager::createInitialAnts()
{
    createAnt(true, glm::vec2(800.0f, 600.0f));
    createAnt(true, glm::vec2(400.0f, 600.0f));
}

void AntManager::createAnt(bool type, glm::vec2 position)
{
    messageBus.send(AntCreationMessage(true, glm::vec2(800.0f, 600.0f)));
    messageBus.send(AntCreationMessage(true, glm::vec2(400.0f, 600.0f)));
}

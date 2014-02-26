#pragma once
#include "messages.h"
#include "ant.h"

class AntManager
{
    public:
        AntManager(fea::MessageBus& bus);

        void createInitialAnts();
        void createAnt(bool type, glm::vec2 position);

    private:
        fea::MessageBus& messageBus;
        std::vector<Ant> ants;
};

#pragma once
#include "messages.h"
#include "ant.h"

class AntManager
{
    public:
        AntManager(fea::MessageBus& bus);

        void createAnt(bool type, bool goingRight, glm::vec2 position);

    private:
        fea::MessageBus& messageBus;
        std::vector<Ant> ants;
};

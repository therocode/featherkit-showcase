#pragma once
#include "messages.h"

class Physics
{
    public:
        Physics(fea::MessageBus& bus);
        ~Physics();

        void update();

    private:
        fea::MessageBus& messageBus;
};

#pragma once
#include "messages.h"
#include "physicsbody.h"

class Physics
{
    public:
        Physics(fea::MessageBus& bus);
        ~Physics();

        void update();

    private:
        fea::MessageBus& messageBus;
};

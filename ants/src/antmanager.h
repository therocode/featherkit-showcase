#pragma once
#include "messages.h"

class AntManager
{
    public:
        AntManager(fea::MessageBus& bus);

    private:
        fea::MessageBus& messageBus;
};

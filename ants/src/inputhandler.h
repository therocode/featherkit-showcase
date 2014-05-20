#pragma once
#include "messages.h"
#include "direction.h"
#include <fea/userinterface.hpp>
#include <fea/ui/sdlinputbackend.hpp>

class InputHandler
{
    public:
        InputHandler(fea::MessageBus& bus);
        void inputLoop();

    private:
        fea::InputHandler input;
        fea::MessageBus& messageBus;

        bool directions[4]; 
};

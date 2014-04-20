#pragma once
#include "messages.h"
#include "direction.h"
#include <featherkit/userinterface.hpp>
#include <featherkit/ui/sdlinputbackend.hpp>

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

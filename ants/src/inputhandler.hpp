#pragma once
#include "messages.hpp"
#include "direction.hpp"
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

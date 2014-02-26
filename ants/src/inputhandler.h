#pragma once
#include "messages.h"
#include "direction.h"
#include <featherkit/userinterface.h>
#include <featherkit/util/input/sdl2inputbackend.h>

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

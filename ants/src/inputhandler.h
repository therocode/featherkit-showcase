#pragma once
#include "messages.h"
#include "direction.h"
#include <featherkit/userinterface.h>
#include <featherkit/util/input/sdl2/sdl2inputbackend.h>

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


InputHandler::InputHandler(fea::MessageBus& bus)
    :   input(new fea::util::SDL2InputBackend()),
        messageBus(bus),
        directions{false, false , false, false}
{
}

void InputHandler::inputLoop()
{
    input.processEvents();
    fea::Event event;

    while(input.pollEvent(event))
    {
        if(event.type == fea::Event::KEYPRESSED)
        {
            if(event.key.code == fea::Keyboard::Q || event.key.code == fea::Keyboard::ESCAPE)
            {
                messageBus.send(QuitMessage());
            }
            else if(event.key.code == fea::Keyboard::W)
            {
                directions[Direction::UP] = true;
            }
            else if(event.key.code == fea::Keyboard::S)
            {
                directions[Direction::DOWN] = true;
            }
            else if(event.key.code == fea::Keyboard::A)
            {
                directions[Direction::LEFT] = true;
            }
            else if(event.key.code == fea::Keyboard::D)
            {
                directions[Direction::RIGHT] = true;
            }
        }
        else if(event.type == fea::Event::KEYRELEASED)
        {
            if(event.key.code == fea::Keyboard::W)
            {
                directions[Direction::UP] = false;
            }
            else if(event.key.code == fea::Keyboard::S)
            {
                directions[Direction::DOWN] = false;
            }
            else if(event.key.code == fea::Keyboard::A)
            {
                directions[Direction::LEFT] = false;
            }
            else if(event.key.code == fea::Keyboard::D)
            {
                directions[Direction::RIGHT] = false;
            }
        }
    }

    if(directions[UP])
        messageBus.send(CameraPositionMessage(glm::vec2(0.0f, -12.0f)));
    if(directions[DOWN])
        messageBus.send(CameraPositionMessage(glm::vec2(0.0f, 12.0f)));
    if(directions[LEFT])
        messageBus.send(CameraPositionMessage(glm::vec2(-12.0f, 0.0f)));
    if(directions[RIGHT])
        messageBus.send(CameraPositionMessage(glm::vec2(12.0f, 0.0f)));

}

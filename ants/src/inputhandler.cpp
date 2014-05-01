#include "inputhandler.h"
#include <iostream>

InputHandler::InputHandler(fea::MessageBus& bus)
    :   input(new fea::SDLInputBackend()),
        messageBus(bus),
        directions({false, false , false, false}) 
{
}

void InputHandler::inputLoop()
{
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
        else if(event.type == fea::Event::MOUSEBUTTONPRESSED)
        {
            if(event.mouseButton.button == fea::Mouse::LEFT)
            {
                messageBus.send(MouseClickMessage(glm::vec2(event.mouseButton.x, event.mouseButton.y)));
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
    
    messageBus.send(MousePositionMessage(glm::vec2(input.getMouseWindowPosition())));
}

#include <featherkit/userinterface.h>
#include <featherkit/util/input/sdl2/sdl2inputbackend.h>

class InputHandler
{
    public:
        InputHandler();
        void inputLoop();

    private:
        fea::InputHandler input;
};


InputHandler::InputHandler()
    :   input(new fea::util::SDL2InputBackend())
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
            {}
                //message to quit
            else if(event.key.code == fea::Keyboard::W)
                // do stuff
            {}
            else if(event.key.code == fea::Keyboard::S)
            {}
                // do stuff
            else if(event.key.code == fea::Keyboard::A)
            {}
                // do stuff
            else if(event.key.code == fea::Keyboard::D)
            {}
                // do stuff
        }
        else if(event.type == fea::Event::KEYRELEASED)
        {
            if(event.key.code == fea::Keyboard::W)
            {}
                // do stuff
            else if(event.key.code == fea::Keyboard::S)
            {}
                // do stuff
            else if(event.key.code == fea::Keyboard::A)
            {}
                // do stuff
            else if(event.key.code == fea::Keyboard::D)
            {}
                // do stuff
        }
    }
}

#pragma once
#include "inputhandler.h"
#include "renderer.h"
#include "imageloader.h"
#include "interpolator.h"
#include "messages.h"
#include <featherkit/structure.h>
#include <featherkit/util/window/sdl2/sdl2windowbackend.h>

class AntApplication
    : public fea::Application,
      public fea::MessageReceiver<QuitMessage>
{
    public:
        AntApplication();
        ~AntApplication();
        void loop() override;
        void setup(const std::vector<std::string>& args) override;
        void destroy() override;

        //messages//
        virtual void handleMessage(const QuitMessage& mess) override;

    private:
        fea::MessageBus messageBus;
        InputHandler input;
        fea::Window window;
        Renderer renderer;
};

AntApplication::AntApplication()
    :   input(messageBus),
        window(new fea::util::SDL2WindowBackend()),
        renderer(messageBus)
{
    messageBus.addSubscriber(*this);
    window.create(fea::VideoMode(800, 600, 32), "ants");
    renderer.setup();
}

AntApplication::~AntApplication()
{
    messageBus.removeSubscriber(*this);
}

void AntApplication::loop()
{
    input.inputLoop();
    renderer.render();
    window.swapBuffers();
}

void AntApplication::setup(const std::vector<std::string>& args)
{
    (void)args;
}

void AntApplication::destroy()
{
    window.close();              
}

void AntApplication::handleMessage(const QuitMessage& mess)
{
    (void)mess;
    quit();
}

#pragma once
#include "inputhandler.h"
#include "imageloader.h"
#include "rainhandler.h"
#include "interpolator.h"
#include "messages.h"
#include <featherkit/render2d.h>
#include <featherkit/structure.h>
#include <featherkit/util/window/sdl2/sdl2windowbackend.h>

class AntApplication
    : public fea::Application,
      public fea::MessageReceiver<QuitMessage>
{
    public:
        AntApplication();
        void loop() override;
        void setup(const std::vector<std::string>& args) override;
        void destroy() override;

        //messages//
        virtual void handleMessage(const QuitMessage& mess) override;

    private:
        fea::MessageBus messageBus;
        InputHandler input;
};

AntApplication::AntApplication()
    :   input(messageBus)
{
}

void AntApplication::loop()
{
    input.inputLoop();
}

void AntApplication::setup(const std::vector<std::string>& args)
{
    (void)args;
}

void AntApplication::destroy()
{
}

void AntApplication::handleMessage(const QuitMessage& mess)
{
    (void)mess;
    quit();
}

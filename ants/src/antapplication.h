#pragma once
#include "inputhandler.h"
#include "antmanager.h"
#include "renderer.h"
#include "imageloader.h"
#include "interpolator.h"
#include "messages.h"
#include "physics.h"
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
        AntManager antManager;
        Physics physics;
        fea::Window window;
        Renderer renderer;
};

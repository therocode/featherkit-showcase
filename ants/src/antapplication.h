#pragma once
#include "inputhandler.h"
#include "antmanager.h"
#include "renderer.h"
#include "imageloader.h"
#include "interpolator.h"
#include "messages.h"
#include "physics.h"
#include "digging.h"
#include <fea/structure.hpp>
#include <fea/ui/sdlwindowbackend.hpp>

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

        void createInitialAnts();

    private:
        fea::MessageBus messageBus;
        InputHandler input;
        AntManager antManager;
        Physics physics;
        fea::Window window;
        Renderer renderer;
        Digging digging;
};

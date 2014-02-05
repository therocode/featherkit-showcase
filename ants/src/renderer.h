#pragma once
#include "messages.h"
#include "interpolator.h"
#include <featherkit/render2d.h>

class Renderer
    :   public fea::MessageReceiver<CameraPositionMessage>
{
    public:
        Renderer(fea::MessageBus& bus);
        ~Renderer();
        void setup();
        void render();

        //messages//
        virtual void handleMessage(const CameraPositionMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        glm::vec2 cameraPosition;
        Interpolator cameraInterpolator;
        fea::Renderer2D renderer;

        fea::Quad quad;
};

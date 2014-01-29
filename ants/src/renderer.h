#pragma once
#include "messages.h"
#include <featherkit/render2d.h>

class Renderer
{
    public:
        Renderer(fea::MessageBus& bus);
        ~Renderer();
        void setup();
    private:
        fea::MessageBus& messageBus;
        fea::Renderer2D renderer;
};

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        renderer(fea::Viewport(800.0f, 600.0f, 0, 0, fea::Camera(800.0f / 2.0f, 600.0f / 2.0f)))
{
}

Renderer::~Renderer()
{
}

void Renderer::setup()
{
    renderer.setup();
}

#pragma once
#include "messages.h"
#include <featherkit/render2d.h>

class Renderer
{
    public:
        Renderer(fea::MessageBus& bus);
        ~Renderer();
        void setup();
        void render();

    private:
        fea::MessageBus& messageBus;
        fea::Renderer2D renderer;

        fea::Quad quad;
};

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        renderer(fea::Viewport(800.0f, 600.0f, 0, 0, fea::Camera(800.0f / 2.0f, 600.0f / 2.0f)))
{
    (void)messageBus;
    quad = fea::Quad(100.0f, 100.0f);
    //quad.setColour(fea::Colour(255, 0, 0));
    quad.setPosition(400.0f, 300.0f);
}

Renderer::~Renderer()
{
}

void Renderer::setup()
{
    renderer.setup();
}

void Renderer::render()
{
    renderer.clear(fea::Colour(0.0f, 0.0f, 1.0f));
    renderer.queue(quad);
    renderer.render();
}

#include "renderer.h"

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        cameraPosition(800.0f/2.0f, 600.0f/2.0f + 300.0f),
        cameraInterpolator(cameraPosition),
        renderer(fea::Viewport(800.0f, 600.0f, 0, 0, fea::Camera(cameraInterpolator.getPosition())))

{
    messageBus.addSubscriber<CameraPositionMessage>(*this);
    quad = fea::Quad(100.0f, 100.0f);
    quad.setColour(fea::Colour(255, 0, 0));
    quad.setPosition(400.0f, 300.0f);
}

Renderer::~Renderer()
{
    messageBus.removeSubscriber<CameraPositionMessage>(*this);
}

void Renderer::setup()
{
    renderer.setup();
}

void Renderer::handleMessage(const CameraPositionMessage& mess)
{
    glm::vec2 vel;
    std::tie(vel) = mess.data;
    cameraPosition += vel;
}

void Renderer::render()
{
    if(cameraPosition.x < 400.0f)
        cameraPosition.x = 400.0f;
    else if(cameraPosition.x > 1200.0f)
        cameraPosition.x = 1200.0f;
    if(cameraPosition.y < 300.0f)
        cameraPosition.y = 300.0f;
    else if(cameraPosition.y > 900.0f)
        cameraPosition.y = 900.0f;

    cameraInterpolator.setPosition(cameraPosition);
    cameraInterpolator.update();
    renderer.getViewport().getCamera().setPosition(cameraInterpolator.getPosition());

    renderer.clear();
    renderer.queue(quad);
    renderer.render();
}

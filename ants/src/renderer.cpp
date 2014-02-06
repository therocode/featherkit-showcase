#include "renderer.h"

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        cameraPosition(800.0f, 600.0f),
        cameraInterpolator(cameraPosition),
        renderer(fea::Viewport(800.0f, 600.0f, 0, 0, fea::Camera(cameraInterpolator.getPosition()))),
        physics(bus)
{
    messageBus.addSubscriber<CameraPositionMessage>(*this);
}

void Renderer::createTexture(const std::string& name, const std::string& path, int width, int height, bool smooth, bool interactive)
{
    fea::Texture texture;
    texture.create(width, height, loader.loadImage(path, width, height).data(), smooth, interactive);
    textures.emplace(name, std::move(texture));
}

Renderer::~Renderer()
{
    messageBus.removeSubscriber<CameraPositionMessage>(*this);
}

void Renderer::setup()
{
    renderer.setup();

    createTexture("dirt", "ants/data/textures/dirt.png", 800, 600, false, true);
    createTexture("dirtbg", "ants/data/textures/dirtbg.png", 800, 600);
    createTexture("ant", "ants/data/textures/ant.png", 200, 100);

    antQuad = fea::Quad(100, 50);
    dirtQuad = fea::Quad(1600, 1200);
    dirtBgQuad = fea::Quad(1600, 1200);
    dirtQuad.setTexture(textures.at("dirt"));
    dirtBgQuad.setTexture(textures.at("dirtbg"));
    antQuad.setTexture(textures.at("ant"));
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

    antQuad.setPosition(800.0f, 600.0f);

    renderer.clear(fea::Colour(255, 0, 0));
    renderer.queue(dirtBgQuad);
    renderer.queue(dirtQuad);
    renderer.queue(antQuad);
    renderer.render();
}

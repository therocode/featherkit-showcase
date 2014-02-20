#include "renderer.h"
#include <iostream>

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        cameraPosition(800.0f, 600.0f),
        cameraInterpolator(cameraPosition),
        renderer(fea::Viewport({800.0f, 600.0f}, {0, 0}, fea::Camera(cameraInterpolator.getPosition())))
{
    messageBus.addSubscriber<CameraPositionMessage>(*this);
    messageBus.addSubscriber<AntPositionMessage>(*this);
    messageBus.addSubscriber<AntCreationMessage>(*this);// take this away
    messageBus.addSubscriber<AntPointsMessage>(*this);
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
    messageBus.removeSubscriber<AntPositionMessage>(*this);
    messageBus.removeSubscriber<AntCreationMessage>(*this);
    messageBus.removeSubscriber<AntPointsMessage>(*this);
}

void Renderer::setup()
{
    renderer.setup();

    createTexture("dirt", "ants/data/textures/dirt.png", 800, 600, false, true);
    createTexture("dirtbg", "ants/data/textures/dirtbg.png", 800, 600);
    createTexture("ant", "ants/data/textures/ant.png", 200, 100);

    dirtQuad = fea::Quad({1600, 1200});
    dirtBgQuad = fea::Quad({1600, 1200});
    dirtQuad.setTexture(textures.at("dirt"));
    dirtBgQuad.setTexture(textures.at("dirtbg"));

    messageBus.send(DirtTextureSetMessage(&textures.at("dirt")));

    pointF = fea::Quad({6, 6});
    pointB = fea::Quad({6, 6});
    pointF.setOrigin({3.0f, 3.0f});
    pointB.setOrigin({3.0f, 3.0f});

    pointF.setPosition({0.0f, 0.0f});
    pointB.setPosition({0.0f, 0.0f});

    pointF.setColour({0, 255, 0});
}

void Renderer::handleMessage(const CameraPositionMessage& mess)
{
    glm::vec2 vel;
    std::tie(vel) = mess.mData;
    cameraPosition += vel;
}

void Renderer::handleMessage(const AntCreationMessage& mess)
{
    bool digging;
    glm::vec2 position;
    std::tie(digging, position) = mess.mData;

    fea::Quad antQuad = fea::Quad({100, 50});
    antQuad.setTexture(textures.at("ant")); // if digging or carrying, different sprites
    antQuad.setOrigin({50.0f, 25.0f});
    antQuad.setPosition(position);
    antQuad.setHFlip(true); // should depend on the velocity

    antQuads.push_back(antQuad);
}

void Renderer::handleMessage(const AntPositionMessage& mess)
{
    int index;
    glm::vec2 position;
    float angle;
    std::tie(index, position, angle) = mess.mData;
    antQuads.at(index).setPosition(position);
    antQuads.at(index).setRotation(angle);
}

void Renderer::handleMessage(const AntPointsMessage& mess)
{
    glm::vec2 positionF;
    glm::vec2 positionB;
    std::tie(positionF, positionB) = mess.mData;
    pointF.setPosition(positionF);
    pointB.setPosition(positionB);
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

    renderer.clear(fea::Colour(0, 125, 255));
    renderer.queue(dirtBgQuad);
    renderer.queue(dirtQuad);
    for(auto& antQuad : antQuads)
    {
        renderer.queue(antQuad);
    }
    //renderer.queue(pointF);
    //renderer.queue(pointB);
    renderer.render();
}

#include "renderer.h"
#include <iostream>

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        cameraPosition(600.0f, 900.0f),
        cameraInterpolator(cameraPosition),
        renderer(fea::Viewport({800.0f, 600.0f}, {0, 0}, fea::Camera(cameraInterpolator.getPosition())))
{
    messageBus.addSubscriber<MouseClickMessage>(*this);
    messageBus.addSubscriber<CameraPositionMessage>(*this);
    messageBus.addSubscriber<AntPositionMessage>(*this);
    messageBus.addSubscriber<AntCreationMessage>(*this);
    messageBus.addSubscriber<AntDeletionMessage>(*this);
}

void Renderer::createTexture(const std::string& name, const std::string& path, int width, int height, bool smooth, bool interactive)
{
    fea::Texture texture;
    texture.create(width, height, loader.loadImage(path, width, height).data(), smooth, interactive);
    textures.emplace(name, std::move(texture));
}

Renderer::~Renderer()
{
    messageBus.removeSubscriber<MouseClickMessage>(*this);
    messageBus.removeSubscriber<CameraPositionMessage>(*this);
    messageBus.removeSubscriber<AntPositionMessage>(*this);
    messageBus.removeSubscriber<AntCreationMessage>(*this);
    messageBus.removeSubscriber<AntDeletionMessage>(*this);
}

void Renderer::setup()
{
    renderer.setup();

    createTexture("dirt", "ants/data/textures/dirt2.png", 800, 600, true, true);
    createTexture("dirtbg", "ants/data/textures/dirtbg2.png", 800, 600, false);
    createTexture("backhills", "ants/data/textures/backhills.png", 800, 600, false);
    createTexture("fronthills", "ants/data/textures/fronthills.png", 800, 600, false);
    createTexture("sky", "ants/data/textures/sky.png", 1, 300);
    createTexture("ant", "ants/data/textures/ant.png", 800, 800);

    dirtQuad = fea::Quad({1600, 1200});
    dirtBgQuad = fea::Quad({1600, 1200});
    frontHillsQuad = fea::Quad({1700, 1200});
    backHillsQuad = fea::Quad({2240, 1200});
    skyQuad = fea::Quad({1600, 1200});

    dirtQuad.setTexture(textures.at("dirt"));
    dirtBgQuad.setTexture(textures.at("dirtbg"));
    frontHillsQuad.setTexture(textures.at("fronthills"));
    backHillsQuad.setTexture(textures.at("backhills"));
    skyQuad.setTexture(textures.at("sky"));

    frontHillsQuad.setParallax({1.1f, 1.0f});
    backHillsQuad.setParallax({1.4f, 1.0f});

    animations.emplace((int)AntType::NORMAL, fea::Animation(glm::vec2(0.0f, 0.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    animations.emplace((int)AntType::DIGGING, fea::Animation(glm::vec2(0.0f, 100.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 8));
    animations.emplace((int)AntType::BLUE, fea::Animation(glm::vec2(0.0f, 200.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    animations.emplace((int)AntType::GREEN, fea::Animation(glm::vec2(0.0f, 300.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    animations.emplace((int)AntType::RED, fea::Animation(glm::vec2(0.0f, 400.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
        
    messageBus.send(DirtTextureSetMessage(&textures.at("dirt")));
}

void Renderer::render()
{
    // camera positions //
    if(cameraPosition.x < 450.0f)
        cameraPosition.x = 450.0f;
    else if(cameraPosition.x > 1150.0f)
        cameraPosition.x = 1150.0f;
    if(cameraPosition.y < 300.0f)
        cameraPosition.y = 300.0f;
    else if(cameraPosition.y > 900.0f)
        cameraPosition.y = 900.0f;

    cameraInterpolator.setPosition(cameraPosition);
    cameraInterpolator.update();
    renderer.getViewport().getCamera().setPosition(cameraInterpolator.getPosition());

    // actual rendering //
    renderer.clear(fea::Color(0, 125, 255));
    renderer.queue(skyQuad);
    renderer.queue(backHillsQuad);
    renderer.queue(frontHillsQuad);
    renderer.queue(dirtBgQuad);
    renderer.queue(dirtQuad);
    for(auto& antSprite : antSprites)
    {
        renderer.queue(antSprite.second.quad);
        antSprite.second.quad.tick();
    }
    renderer.render();
}

void Renderer::handleMessage(const CameraPositionMessage& mess)
{
    glm::vec2 vel;
    std::tie(vel) = mess.mData;
    cameraPosition += vel;
}

void Renderer::handleMessage(const MouseClickMessage& mess)
{
    glm::vec2 mousePos;
    std::tie(mousePos) = mess.mData;
    mousePos += cameraPosition;
    mousePos -= glm::vec2(400.0f, 300.0f);
    std::cout << "at " << mousePos.x << ", " << mousePos.y << "\n";
}

void Renderer::handleMessage(const AntCreationMessage& mess)
{
    size_t id;
    AntType type;
    bool goingRight;
    glm::vec2 position;
    std::tie(id, type, goingRight, position, std::ignore) = mess.mData;

    fea::AnimatedQuad antQuad = fea::AnimatedQuad({50, 25});
    antQuad.setTexture(textures.at("ant")); 
    antQuad.setAnimation(animations.at(type));
    antQuad.setOrigin({25.0f, 12.5f});
    antQuad.setPosition(position);
    antQuad.setHFlip(goingRight);

    antSprites.emplace(id, AntSprite(antQuad, type));  
}

void Renderer::handleMessage(const AntDeletionMessage& mess)
{
    int index;
    std::tie(index) = mess.mData;

    antSprites.erase(index);
}

void Renderer::handleMessage(const AntPositionMessage& mess)
{
    size_t index;
    glm::vec2 position;
    float angle;
    std::tie(index, position, angle) = mess.mData;
    antSprites.at(index).quad.setPosition(position);
    antSprites.at(index).quad.setRotation(angle);
}

#include "renderer.h"
#include <iostream>

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        cameraPosition(600.0f, 900.0f),
        cameraInterpolator(cameraPosition),
        renderer(fea::Viewport({800.0f, 600.0f}, {0, 0})),
        gui(bus),
        renderTargetVP(fea::Viewport({1600.0f, 600.0f}, {0, 0}, fea::Camera({800.0f, 300.0f}))),
        guiCam({0.0f, 0.0f})
{
    messageBus.addSubscriber<MouseClickMessage>(gui);
    messageBus.addSubscriber<MousePositionMessage>(gui);
    messageBus.addSubscriber<CameraPositionMessage>(*this);
    messageBus.addSubscriber<AntPositionMessage>(*this);
    messageBus.addSubscriber<AntCreationMessage>(*this);
    messageBus.addSubscriber<AntDeletionMessage>(*this);
    messageBus.addSubscriber<GuiButtonClickedMessage>(*this);

    renderStateButton = ButtonType::B_DEFAULT;
    renderStates.emplace(ButtonType::B_DEFAULT,      RenderState(1.00f, glm::vec2( 450.0f,  300.0f), glm::vec2(1150.0,  900.0f)));
    renderStates.emplace(ButtonType::B_INTERACTIVE,  RenderState(1.50f, glm::vec2(1000.0f, 1000.0f), glm::vec2(1000.0, 1000.0f)));
    renderStates.emplace(ButtonType::B_COLOUR_BLEND, RenderState(1.20f, glm::vec2( 500.0f,  950.0f), glm::vec2( 500.0,  950.0f)));
    renderStates.emplace(ButtonType::B_PARALLAX,     RenderState(0.40f, glm::vec2( 450.0f,  600.0f), glm::vec2(1150.0,  600.0f)));
    renderStates.emplace(ButtonType::B_ANIMATION,    RenderState(6.00f, glm::vec2( 450.0f,  300.0f), glm::vec2(1150.0,  900.0f)));
    renderStates.emplace(ButtonType::B_TEXT,         RenderState(1.00f, glm::vec2( 450.0f,  300.0f), glm::vec2(1150.0,  900.0f)));
    renderStates.emplace(ButtonType::B_CAMERA,       RenderState(0.52f, glm::vec2( 800.0f,  600.0f), glm::vec2( 800.0,  600.0f)));

    cameraZoom = 2.0f;
}

void Renderer::createTexture(const std::string& name, const std::string& path, int width, int height, bool smooth, bool interactive)
{
    fea::Texture texture;
    texture.create(width, height, loader.loadImage(path, width, height).data(), smooth, interactive);
    textures.emplace(name, std::move(texture));
}

Renderer::~Renderer()
{
    messageBus.removeSubscriber<MouseClickMessage>(gui);
    messageBus.removeSubscriber<MousePositionMessage>(gui);
    messageBus.removeSubscriber<CameraPositionMessage>(*this);
    messageBus.removeSubscriber<AntPositionMessage>(*this);
    messageBus.removeSubscriber<AntCreationMessage>(*this);
    messageBus.removeSubscriber<AntDeletionMessage>(*this);
    messageBus.removeSubscriber<GuiButtonClickedMessage>(*this);
}

void Renderer::setup()
{
    renderer.setup();

    gui.setup();

    createTextures();
    setupQuads();
    createAnimations();
    setupRenderTarget();

    messageBus.send(DirtTextureSetMessage({&textures.at("dirt")}));
}

void Renderer::render()
{
    updateCamera(); 
    cloudHandler.update();
    gui.update();

    renderRenderTarget();
    renderScene();

    renderGUI();    
}

void Renderer::handleMessage(const CameraPositionMessage& mess)
{
    cameraPosition += mess.mVelocityToAdd;
}

void Renderer::handleMessage(const AntCreationMessage& mess)
{
    fea::AnimatedQuad antQuad = fea::AnimatedQuad({50, 25});
    antQuad.setTexture(textures.at("ant")); 
    antQuad.setAnimation(animations.at(mess.mAntType));
    antQuad.setOrigin({25.0f, 12.5f});
    antQuad.setPosition(mess.mPosition);
    antQuad.setHFlip(mess.mGoingRight);

    antSprites.emplace(mess.mAntId, AntSprite(antQuad, mess.mAntType));  
}

void Renderer::handleMessage(const AntDeletionMessage& mess)
{
    antSprites.erase(mess.mAntId);
}

void Renderer::handleMessage(const AntPositionMessage& mess)
{
    antSprites.at(mess.mAntId).quad.setPosition(mess.mOriginPosition);
    antSprites.at(mess.mAntId).quad.setRotation(mess.mAngle);
}

void Renderer::handleMessage(const GuiButtonClickedMessage& mess)
{
    renderStateButton = mess.mButtonType;
}

////

void Renderer::createTextures()
{
    createTexture("dirt", "ants/data/textures/dirt2.png", 800, 600, true, true);
    createTexture("dirtbg", "ants/data/textures/dirtbg2.png", 800, 600, false);
    createTexture("backhills", "ants/data/textures/backhills.png", 800, 600, false);
    createTexture("fronthills", "ants/data/textures/fronthills.png", 800, 600, false);
    createTexture("sky", "ants/data/textures/sky.png", 1, 300);
    createTexture("cloud1", "ants/data/textures/cloud1.png", 320, 200);
    createTexture("cloud2", "ants/data/textures/cloud2.png", 320, 200);
    createTexture("cloud3", "ants/data/textures/cloud3.png", 320, 200);
    createTexture("cloud4", "ants/data/textures/cloud4.png", 160, 200);
    createTexture("ant", "ants/data/textures/ant.png", 800, 800);
    createTexture("darkness", "ants/data/textures/darkness.png", 800, 300);
    createTexture("halo", "ants/data/textures/halo.png", 145, 145);
}

void Renderer::setupQuads()
{
    dirtQuad = fea::Quad({1600, 1200});
    dirtBgQuad = fea::Quad({1600, 1200});
    frontHillsQuad = fea::Quad({1700, 1200});
    backHillsQuad = fea::Quad({2240, 1200});
    skyQuad = fea::Quad({1600, 1200});
    fea::Quad cloud1Quad = fea::Quad({320, 200});
    fea::Quad cloud2Quad = fea::Quad({320, 200});
    fea::Quad cloud3Quad = fea::Quad({320, 200});
    fea::Quad cloud4Quad = fea::Quad({160, 200});

    dirtQuad.setTexture(textures.at("dirt"));
    dirtBgQuad.setTexture(textures.at("dirtbg"));
    frontHillsQuad.setTexture(textures.at("fronthills"));
    backHillsQuad.setTexture(textures.at("backhills"));
    skyQuad.setTexture(textures.at("sky"));
    cloud1Quad.setTexture(textures.at("cloud1"));
    cloud2Quad.setTexture(textures.at("cloud2"));
    cloud3Quad.setTexture(textures.at("cloud3"));
    cloud4Quad.setTexture(textures.at("cloud4"));

    cloudQuads.push_back(cloud1Quad);
    cloudQuads.push_back(cloud2Quad);
    cloudQuads.push_back(cloud3Quad);
    cloudQuads.push_back(cloud4Quad);

    frontHillsQuad.setParallax({0.7f, 1.0f});
    backHillsQuad.setParallax({0.5f, 1.0f});
    frontHillsQuad.setPosition({-300.0f, 0.0f});
    backHillsQuad.setPosition({-800.0f, 0.0f});
}

void Renderer::createAnimations()
{
    animations.emplace((int)AntType::NORMAL, fea::Animation(glm::vec2(0.0f, 0.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    animations.emplace((int)AntType::DIGGING, fea::Animation(glm::vec2(0.0f, 100.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 8));
    animations.emplace((int)AntType::BLUE, fea::Animation(glm::vec2(0.0f, 200.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    animations.emplace((int)AntType::GREEN, fea::Animation(glm::vec2(0.0f, 300.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    animations.emplace((int)AntType::RED, fea::Animation(glm::vec2(0.0f, 400.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
}

void Renderer::setupRenderTarget()
{
    lightingTarget.create(1600, 600);
    lightingQuad = fea::Quad({1600, 600});
    darknessQuad = fea::Quad({1600, 600});
    largeHalo = fea::Quad({290, 290});
    smallHalo = fea::Quad({200, 200});

    lightingQuad.setPosition({0, 600});
    lightingQuad.setVFlip(true);
    lightingQuad.setTexture(lightingTarget.getTexture());
    darknessQuad.setTexture(textures.at("darkness"));
    largeHalo.setTexture(textures.at("halo"));
    smallHalo.setTexture(textures.at("halo"));
}

void Renderer::updateCamera()
{
    RenderState currentState = renderStates.at(renderStateButton);

    cameraZoom = defaultSceneCam.getZoom().x;
    if(fabs(cameraZoom - currentState.cameraZoomTarget) < 0.01f)
    {
        cameraZoom = currentState.cameraZoomTarget;
    }
    else if(cameraZoom < currentState.cameraZoomTarget)
    {
        cameraZoom += 0.01f;
    }
    else if(cameraZoom > currentState.cameraZoomTarget)
    {
        cameraZoom -= 0.01f;
    }
    defaultSceneCam.setZoom({cameraZoom, cameraZoom});

    float leftBound = currentState.topLeftCameraBoundary.x;
    float rightBound = currentState.bottomRightCameraBoundary.x;
    float topBound = currentState.topLeftCameraBoundary.y;
    float bottomBound = currentState.bottomRightCameraBoundary.y;

    if(cameraPosition.x < leftBound)
        cameraPosition.x = leftBound;
    else if(cameraPosition.x > rightBound)
        cameraPosition.x = rightBound;
    if(cameraPosition.y < topBound)
        cameraPosition.y = topBound;
    else if(cameraPosition.y > bottomBound)
        cameraPosition.y = bottomBound;
    
    cameraInterpolator.setPosition(cameraPosition);
    cameraInterpolator.update();
    defaultSceneCam.setPosition(cameraInterpolator.getPosition());

    defaultSceneVP = renderer.getViewport(); // might have to move this or change this or something
}

void Renderer::renderRenderTarget()
{
    renderer.setViewport(renderTargetVP);
    renderer.clear(lightingTarget, fea::Color(0, 0, 0, 255));
    renderer.queue(darknessQuad);
    renderer.setBlendMode(fea::BlendMode::ADD);
    //R
    largeHalo.setColor(fea::Color(255, 0, 0, 70));
    largeHalo.setPosition({1155, 325});
    renderer.queue(largeHalo);
    //G
    largeHalo.setColor(fea::Color(0, 255, 0, 70));
    largeHalo.setPosition({668, 311});
    renderer.queue(largeHalo);
    //B
    largeHalo.setColor(fea::Color(0, 0, 255, 70));
    largeHalo.setPosition({185, 312});
    renderer.queue(largeHalo);
    //small halos
    for(auto& antSprite : antSprites)
    {
        if(antSprite.second.type == AntType::RED)
        {
            smallHalo.setColor(fea::Color(255, 0, 0, 70));
            smallHalo.setPosition(antSprite.second.quad.getPosition() - glm::vec2(100.0f, 700.0f));
            renderer.queue(smallHalo);
        }
        else if(antSprite.second.type == AntType::GREEN)
        {
            smallHalo.setColor(fea::Color(0, 255, 0, 70));
            smallHalo.setPosition(antSprite.second.quad.getPosition() - glm::vec2(100.0f, 700.0f));
            renderer.queue(smallHalo);
        }
        else if(antSprite.second.type == AntType::BLUE)
        {
            smallHalo.setColor(fea::Color(0, 0, 255, 70));
            smallHalo.setPosition(antSprite.second.quad.getPosition() - glm::vec2(100.0f, 700.0f));
            renderer.queue(smallHalo);
        }
    }
    renderer.render(lightingTarget);
}

void Renderer::renderScene()
{
    renderer.setBlendMode(fea::BlendMode::ALPHA);
    renderer.setViewport(defaultSceneVP);
    renderer.getViewport().setCamera(defaultSceneCam);
    renderer.clear(fea::Color(0, 0, 0));
    renderer.queue(skyQuad);
    for(size_t i = 0; i < cloudHandler.getCloudPositions().size(); i++)
    {
        cloudQuads.at(i).setPosition(cloudHandler.getCloudPositions().at(i));
        renderer.queue(cloudQuads.at(i));
    }
    renderer.queue(backHillsQuad);
    renderer.queue(frontHillsQuad);
    renderer.queue(dirtBgQuad);
    renderer.queue(dirtQuad);
    for(auto& antSprite : antSprites)
    {
        renderer.queue(antSprite.second.quad);
        antSprite.second.quad.tick();
    }
    renderer.setBlendMode(fea::BlendMode::MULTIPLY);
    renderer.queue(lightingQuad);
    renderer.setBlendMode(fea::BlendMode::ALPHA);
    renderer.render();
}

void Renderer::renderGUI()
{
    renderer.getViewport().setCamera(guiCam);
    for(auto& drawable : gui.getDrawables())
    {
        //fea::Drawable2D copy = *drawable;
        //copy.setPosition(asdf);
        renderer.queue(*drawable);
    }
    renderer.render();
}

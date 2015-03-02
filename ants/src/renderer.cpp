#include "renderer.hpp"
#include <iostream>

Renderer::Renderer(fea::MessageBus& bus)
    :   messageBus(bus),
        cameraPosition(600.0f, 900.0f),
        cameraInterpolator(cameraPosition),
        renderer(fea::Viewport({800.0f, 600.0f}, {0, 0})),
        mScene(bus),
        mGui(bus),
        renderTargetVP(fea::Viewport({1600.0f, 600.0f}, {0, 0}, fea::Camera({800.0f, 300.0f}))),
        mGuiCam({0.0f, 0.0f})
{
    fea::subscribe(messageBus, *this);

    renderStateButton = ButtonType::B_DEFAULT;
    renderStates.emplace(ButtonType::B_DEFAULT,      RenderState(1.00f, glm::vec2( 450.0f,  300.0f), glm::vec2(1150.0,  900.0f)));
    renderStates.emplace(ButtonType::B_INTERACTIVE,  RenderState(1.50f, glm::vec2(1000.0f, 1000.0f), glm::vec2(1000.0, 1000.0f)));
    renderStates.emplace(ButtonType::B_COLOUR_BLEND, RenderState(1.20f, glm::vec2( 500.0f,  950.0f), glm::vec2( 500.0,  950.0f)));
    renderStates.emplace(ButtonType::B_PARALLAX,     RenderState(0.40f, glm::vec2( 450.0f,  600.0f), glm::vec2(1150.0,  600.0f)));
    renderStates.emplace(ButtonType::B_ANIMATION,    RenderState(4.00f, glm::vec2( 450.0f,  300.0f), glm::vec2(1150.0,  900.0f)));
    renderStates.emplace(ButtonType::B_TEXT,         RenderState(1.00f, glm::vec2( 450.0f,  300.0f), glm::vec2(1150.0,  900.0f)));
    renderStates.emplace(ButtonType::B_CAMERA,       RenderState(0.52f, glm::vec2( 800.0f,  600.0f), glm::vec2( 800.0,  600.0f)));

    cameraZoom = 2.0f;

}

void Renderer::setup()
{
    renderer.setup();

    mGui.setup();
    mWiggleText.setup();
    mScene.setup();
}

void Renderer::render()
{
    updateCamera(); 
    mScene.update();
    mGui.update();

    renderLighting();
    renderScene();
    renderGUI();    

    if(renderStateButton == ButtonType::B_TEXT)
    {
        renderTextState();
    }
}

void Renderer::handleMessage(const CameraPositionMessage& mess)
{
    cameraPosition += mess.mVelocityToAdd;
}

void Renderer::handleMessage(const GuiButtonClickedMessage& mess)
{
    renderStateButton = mess.mButtonType;
}

////

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
        if(cameraZoom > 1.50f)
        {
            cameraZoom += 0.04f;
        }
        else
        {
            cameraZoom += 0.01f;
        }
    }
    else if(cameraZoom > currentState.cameraZoomTarget)
    {
        if(cameraZoom > 1.50f)
        {
            cameraZoom -= 0.04f;
        }
        else
        {
            cameraZoom -= 0.01f;
        }
    }
    defaultSceneCam.setZoom({cameraZoom, cameraZoom});

    if(renderStateButton != B_ANIMATION)
    {
        float leftBound = currentState.topLeftCameraBoundary.x;
        float rightBound = currentState.bottomRightCameraBoundary.x;
        float topBound = currentState.topLeftCameraBoundary.y;
        float bottomBound = currentState.bottomRightCameraBoundary.y;

        if(cameraPosition.x < leftBound)
        {
            cameraPosition.x = leftBound;
        }
        else if(cameraPosition.x > rightBound)
        {
            cameraPosition.x = rightBound;
        }
        if(cameraPosition.y < topBound)
        {
            cameraPosition.y = topBound;
        }
        else if(cameraPosition.y > bottomBound)
        {
            cameraPosition.y = bottomBound;
        }
    }
    else
    {   // follow an ant
        
        // if animationId has expired, then get a new one! //
        if(mScene.getAntSprites().count(animationFollowAntId) == 0)
        {
            animationFollowAntId = mScene.getAntSprites().begin()->first;
        }
        AntSprite antSprite = mScene.getAntSprites().at(animationFollowAntId);
        float xalle = antSprite.mQuad.getPosition().x;
        float yalle = antSprite.mQuad.getPosition().y;
        
        if(cameraPosition.x < xalle)
        {
            cameraPosition.x = xalle;
        }
        else if(cameraPosition.x > xalle)
        {
            cameraPosition.x = xalle;
        }
        if(cameraPosition.y < yalle)
        {
            cameraPosition.y = yalle;
        }
        else if(cameraPosition.y > yalle)
        {
            cameraPosition.y = yalle;
        }
    }
    
    cameraInterpolator.setPosition(cameraPosition);
    cameraInterpolator.update();
    defaultSceneCam.setPosition(cameraInterpolator.getPosition());

    defaultSceneVP = renderer.getViewport(); // might have to move this or change this or something
}

void Renderer::renderLighting()
{
    fea::RenderTarget& lightingTarget = mScene.getLightingRenderTarget();
    std::unordered_map<LightingQuadType, fea::Quad>& lightingQuads = mScene.getLightingQuads();
    std::unordered_map<size_t, AntSprite>& antSprites = mScene.getAntSprites();

    renderer.setViewport(renderTargetVP);
    renderer.clear(lightingTarget, fea::Color(0, 0, 0, 255));
    renderer.queue(lightingQuads.at(LightingQuadType::DARK));
    renderer.setBlendMode(fea::BlendMode::ADD);

    // large halos //
    fea::Quad& largeHalo = lightingQuads.at(LightingQuadType::LARGE_HALO);
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

    // small halos //
    fea::Quad& smallHalo = lightingQuads.at(LightingQuadType::SMALL_HALO);
    for(auto& antSprite : antSprites)
    {
        if(antSprite.second.mType == AntType::RED)
        {
            smallHalo.setColor(fea::Color(255, 0, 0, 70));
            smallHalo.setPosition(antSprite.second.mQuad.getPosition() - glm::vec2(100.0f, 700.0f));
            renderer.queue(smallHalo);
        }
        else if(antSprite.second.mType == AntType::GREEN)
        {
            smallHalo.setColor(fea::Color(0, 255, 0, 70));
            smallHalo.setPosition(antSprite.second.mQuad.getPosition() - glm::vec2(100.0f, 700.0f));
            renderer.queue(smallHalo);
        }
        else if(antSprite.second.mType == AntType::BLUE)
        {
            smallHalo.setColor(fea::Color(0, 0, 255, 70));
            smallHalo.setPosition(antSprite.second.mQuad.getPosition() - glm::vec2(100.0f, 700.0f));
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
    renderer.clear(fea::Color::Black);

    for(auto& drawable : mScene.getLandscapeQuads())
    {
        renderer.queue(drawable);
    }

    for(auto& antSprite : mScene.getAntSprites())
    {
        renderer.queue(antSprite.second.mQuad);
        antSprite.second.mQuad.tick();
    }

    for(auto& cloud : mScene.getCloudQuads())
    {
        renderer.queue(cloud);
    }

    // lighting //
    fea::Quad lightingQuad = fea::Quad({1600, 600});
    lightingQuad.setTexture(mScene.getLightingRenderTarget().getTexture());
    lightingQuad.setVFlip(true);
    lightingQuad.setPosition({0.0f, 600.0f});
    renderer.setBlendMode(fea::BlendMode::MULTIPLY);
    renderer.queue(lightingQuad);
    renderer.setBlendMode(fea::BlendMode::ALPHA);

    renderer.render();
}

void Renderer::renderGUI()
{
    renderer.getViewport().setCamera(mGuiCam);
    for(auto& drawable : mGui.getDrawables())
    {
        renderer.queue(*drawable);
    }
    renderer.render();
}

void Renderer::renderTextState()
{
    mWiggleText.wiggle();
    for(auto& drawable : mWiggleText.getDrawables())
    {
        renderer.queue(*drawable);
    }
    renderer.render();
}

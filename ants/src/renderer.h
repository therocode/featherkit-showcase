// holy shit refactor this //
#pragma once
#include "renderstate.h"
#include "messages.h"
#include "interpolator.h"
#include "wiggletext.h"
#include "scene.h"
#include "gui.h"
#include <fea/render2d.hpp>

class Renderer
    :   public fea::MessageReceiver<CameraPositionMessage, GuiButtonClickedMessage>
{
    public:
        Renderer(fea::MessageBus& bus);
        void setup();
        void render();

        //messages//
        virtual void handleMessage(const CameraPositionMessage& mess) override;
        virtual void handleMessage(const GuiButtonClickedMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        glm::vec2 cameraPosition;
        Interpolator cameraInterpolator;
        fea::Renderer2D renderer;

        Scene mScene;
        GUI mGui;
        WiggleText mWiggleText;
        std::unordered_map<ButtonType, RenderState> renderStates;
        ButtonType renderStateButton;

        float cameraZoom;

        fea::Viewport defaultSceneVP;
        const fea::Viewport renderTargetVP;
        fea::Camera defaultSceneCam; 
        const fea::Camera mGuiCam;

        void createTextures();
        void setupQuads();
        void createAnimations();
        void setupRenderTarget();

        void updateCamera();
        void renderLighting();
        void renderScene();
        void renderGUI();
        void renderTextState();
};

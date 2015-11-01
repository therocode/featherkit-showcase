#pragma once
#include "renderstate.hpp"
#include "messages.hpp"
#include "interpolator.hpp"
#include "wiggletext.hpp"
#include "scene.hpp"
#include "gui.hpp"
#include <fea/render2d.hpp>

class Renderer
    :   public fea::MessageReceiver<CameraPositionMessage, GuiButtonClickedMessage>
{
    public:
        Renderer(fea::MessageBus& bus);
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

        size_t animationFollowAntId;

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

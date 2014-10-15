#pragma once
#include "renderstate.h"
#include "messages.h"
#include "interpolator.h"
#include "imageloader.h"
#include "antsprite.h"
#include "cloudhandler.h"
#include "gui.h"
#include <fea/render2d.hpp>

class Renderer
    :   public fea::MessageReceiver<CameraPositionMessage, AntPositionMessage, AntCreationMessage, AntDeletionMessage, GuiButtonClickedMessage>
{
    public:
        Renderer(fea::MessageBus& bus);
        void setup();
        void render();

        //messages//
        virtual void handleMessage(const CameraPositionMessage& mess) override;
        virtual void handleMessage(const AntPositionMessage& mess) override;
        virtual void handleMessage(const AntCreationMessage& mess) override;
        virtual void handleMessage(const AntDeletionMessage& mess) override;
        virtual void handleMessage(const GuiButtonClickedMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        glm::vec2 cameraPosition;
        Interpolator cameraInterpolator;
        fea::Renderer2D renderer;
        GUI gui;
        std::unordered_map<ButtonType, RenderState> renderStates;
        ButtonType renderStateButton;
        float cameraZoom;

        ImageLoader loader;
        std::unordered_map<std::string, fea::Texture> textures;
        void createTexture(const std::string& name, const std::string& path, int width, int height, bool smooth = false, bool interactive = false);

        std::unordered_map<size_t, AntSprite> antSprites;
        std::unordered_map<int, fea::Animation> animations;

        fea::Quad dirtQuad;
        fea::Quad dirtBgQuad;
        fea::Quad frontHillsQuad;
        fea::Quad backHillsQuad;
        fea::Quad skyQuad;
        std::vector<fea::Quad> cloudQuads;
        CloudHandler cloudHandler;

        fea::Viewport defaultSceneVP;
        const fea::Viewport renderTargetVP;
        fea::Camera defaultSceneCam; 
        const fea::Camera guiCam;
        fea::RenderTarget lightingTarget;
        fea::Quad lightingQuad;
        fea::Quad darknessQuad;
        fea::Quad largeHalo;
        fea::Quad smallHalo;

        void createTextures();
        void setupQuads();
        void createAnimations();
        void setupRenderTarget();

        void updateCamera();
        void renderRenderTarget();
        void renderScene();
        void renderGUI();
};

#pragma once
#include "messages.h"
#include "interpolator.h"
#include "imageloader.h"
#include "antsprite.h"
#include "cloudhandler.h"
#include "gui.h"
#include <featherkit/render2d.hpp>

class Renderer
    :   public fea::MessageReceiver<CameraPositionMessage>,
        public fea::MessageReceiver<MouseClickMessage>,
        public fea::MessageReceiver<AntPositionMessage>,
        public fea::MessageReceiver<AntCreationMessage>,
        public fea::MessageReceiver<AntDeletionMessage>
{
    public:
        Renderer(fea::MessageBus& bus);
        ~Renderer();
        void setup();
        void render();

        //messages//
        virtual void handleMessage(const MouseClickMessage& mess) override;
        virtual void handleMessage(const CameraPositionMessage& mess) override;
        virtual void handleMessage(const AntPositionMessage& mess) override;
        virtual void handleMessage(const AntCreationMessage& mess) override;
        virtual void handleMessage(const AntDeletionMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        glm::vec2 cameraPosition;
        Interpolator cameraInterpolator;
        fea::Renderer2D renderer;
        GUI gui;

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

        fea::Viewport sceneVP;
        const fea::Viewport targetVP;
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

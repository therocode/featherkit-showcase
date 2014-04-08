#pragma once
#include "messages.h"
#include "interpolator.h"
#include "imageloader.h"
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

        ImageLoader loader;
        std::unordered_map<std::string, fea::Texture> textures;
        void createTexture(const std::string& name, const std::string& path, int width, int height, bool smooth = false, bool interactive = false);

        std::unordered_map<size_t, fea::AnimatedQuad> antQuads;
        fea::Animation antAnimation;

        fea::Quad dirtQuad;
        fea::Quad dirtBgQuad;
        fea::Quad skyQuad;
};

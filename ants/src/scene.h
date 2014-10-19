#pragma once
#include "cloudhandler.h"
#include "imageloader.h"
#include "antsprite.h"
#include "messages.h"
#include "lightingquadtype.h"
#include <fea/rendering/rendertarget.hpp>

class Scene
    :   public fea::MessageReceiver<AntPositionMessage, AntCreationMessage, AntDeletionMessage>
{
    public:
        Scene(fea::MessageBus& bus);
        void setup();
        void update();
        const std::vector<fea::Quad>& getLandscapeQuads();
        const std::vector<fea::Quad>& getCloudQuads();
        std::unordered_map<size_t, AntSprite>& getAntSprites();
        std::unordered_map<LightingQuadType, fea::Quad>& getLightingQuads();;
        fea::RenderTarget& getLightingRenderTarget();

        virtual void handleMessage(const AntPositionMessage& mess) override;
        virtual void handleMessage(const AntCreationMessage& mess) override;
        virtual void handleMessage(const AntDeletionMessage& mess) override;

    private:
        fea::MessageBus& mBus;
        ImageLoader mLoader;

        void createTexture(const std::string& name, const std::string& path, int width, int height, bool smooth = false, bool interactive = false);
        std::unordered_map<std::string, fea::Texture> mTextures;

        std::vector<fea::Quad> mLandscapeQuads;
        std::unordered_map<size_t, AntSprite> mAntSprites;
        std::unordered_map<int, fea::Animation> mAnimations;
        CloudHandler mCloudHandler;
        fea::RenderTarget mLightingTarget;
        std::unordered_map<LightingQuadType, fea::Quad> mLightingQuads;

        void createTextures();
        void setupQuads();
        void createAnimations();
};

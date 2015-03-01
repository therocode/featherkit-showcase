#pragma once
#include "cloudhandler.hpp"
#include "imageloader.hpp"
#include "antsprite.hpp"
#include "messages.hpp"
#include "lightingquadtype.hpp"
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

        void handleMessage(const AntPositionMessage& mess) override;
        void handleMessage(const AntCreationMessage& mess) override;
        void handleMessage(const AntDeletionMessage& mess) override;

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

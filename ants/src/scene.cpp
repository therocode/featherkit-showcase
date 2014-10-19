#include "scene.h"

Scene::Scene(fea::MessageBus& bus)
    :   mBus(bus)
{
    fea::subscribe(mBus, *this);
}

void Scene::setup()
{
    createTextures();
    setupQuads();
    createAnimations();

    mBus.send(DirtTextureSetMessage({&mTextures.at("dirt")}));
}

void Scene::update()
{
    mCloudHandler.update();
}

const std::vector<fea::Quad>& Scene::getLandscapeQuads()
{
    return mLandscapeQuads;
}

const std::vector<fea::Quad>& Scene::getCloudQuads()
{
    return mCloudHandler.getCloudQuads();
}

std::unordered_map<size_t, AntSprite>& Scene::getAntSprites()
{
    return mAntSprites;
}

std::unordered_map<LightingQuadType, fea::Quad>& Scene::getLightingQuads()
{
    return mLightingQuads;
}

fea::RenderTarget& Scene::getLightingRenderTarget()
{
    return mLightingTarget;
}

void Scene::createTexture(const std::string& name, const std::string& path, int width, int height, bool smooth, bool interactive)
{
    fea::Texture texture;
    texture.create(width, height, mLoader.loadImage(path).data(), smooth, interactive);
    mTextures.emplace(name, std::move(texture));
}

void Scene::createTextures()
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

void Scene::setupQuads()
{
    fea::Quad dirtQuad = fea::Quad({1600, 1200});
    fea::Quad dirtBgQuad = fea::Quad({1600, 1200});
    fea::Quad frontHillsQuad = fea::Quad({1700, 1200});
    fea::Quad backHillsQuad = fea::Quad({2240, 1200});
    fea::Quad skyQuad = fea::Quad({1600, 1200});

    dirtQuad.setTexture(mTextures.at("dirt"));
    dirtBgQuad.setTexture(mTextures.at("dirtbg"));
    frontHillsQuad.setTexture(mTextures.at("fronthills"));
    backHillsQuad.setTexture(mTextures.at("backhills"));
    skyQuad.setTexture(mTextures.at("sky"));

    frontHillsQuad.setParallax({0.7f, 1.0f});
    backHillsQuad.setParallax({0.5f, 1.0f});
    frontHillsQuad.setPosition({-300.0f, 0.0f});
    backHillsQuad.setPosition({-800.0f, 0.0f});

    mLandscapeQuads.push_back(skyQuad);
    mLandscapeQuads.back().setTexture(mTextures.at("sky"));
    mLandscapeQuads.push_back(backHillsQuad);
    mLandscapeQuads.push_back(frontHillsQuad);
    mLandscapeQuads.push_back(dirtBgQuad);
    mLandscapeQuads.push_back(dirtQuad);

    fea::Quad cloud1Quad = fea::Quad({320, 200});
    fea::Quad cloud2Quad = fea::Quad({320, 200});
    fea::Quad cloud3Quad = fea::Quad({320, 200});
    fea::Quad cloud4Quad = fea::Quad({160, 200});
    cloud1Quad.setTexture(mTextures.at("cloud1"));
    cloud2Quad.setTexture(mTextures.at("cloud2"));
    cloud3Quad.setTexture(mTextures.at("cloud3"));
    cloud4Quad.setTexture(mTextures.at("cloud4"));
    mCloudHandler.addCloud(cloud1Quad);
    mCloudHandler.addCloud(cloud2Quad);
    mCloudHandler.addCloud(cloud3Quad);
    mCloudHandler.addCloud(cloud4Quad);

    mLightingTarget.create(1600, 600);
    fea::Quad mLightingQuad = fea::Quad({1600, 600});
    fea::Quad mDarknessQuad = fea::Quad({1600, 600});
    fea::Quad mLargeHalo = fea::Quad({290, 290});
    fea::Quad mSmallHalo = fea::Quad({200, 200});

    mLightingQuad.setPosition({0, 600});
    mLightingQuad.setVFlip(true);
    mLightingQuad.setTexture(mLightingTarget.getTexture());
    mDarknessQuad.setTexture(mTextures.at("darkness"));
    mLargeHalo.setTexture(mTextures.at("halo"));
    mSmallHalo.setTexture(mTextures.at("halo"));

    mLightingQuads.emplace(LightingQuadType::LIGHT, mLightingQuad);
    mLightingQuads.emplace(LightingQuadType::DARK, mDarknessQuad);
    mLightingQuads.emplace(LightingQuadType::LARGE_HALO, mLargeHalo);
    mLightingQuads.emplace(LightingQuadType::SMALL_HALO, mSmallHalo);
}

void Scene::createAnimations()
{
    mAnimations.emplace((int)AntType::NORMAL,  fea::Animation(glm::vec2(0.0f, 0.0f         ), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    mAnimations.emplace((int)AntType::DIGGING, fea::Animation(glm::vec2(0.0f, 100.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4,  8));
    mAnimations.emplace((int)AntType::BLUE,    fea::Animation(glm::vec2(0.0f, 200.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    mAnimations.emplace((int)AntType::GREEN,   fea::Animation(glm::vec2(0.0f, 300.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
    mAnimations.emplace((int)AntType::RED,     fea::Animation(glm::vec2(0.0f, 400.0f/800.0f), glm::vec2(200.0f/800.0f, 100.0f/800.0f), 4, 16));
}

void Scene::handleMessage(const AntCreationMessage& mess)
{
    fea::AnimatedQuad antQuad = fea::AnimatedQuad({50, 25});
    antQuad.setTexture(mTextures.at("ant")); 
    antQuad.setAnimation(mAnimations.at(mess.mAntType));
    antQuad.setOrigin({25.0f, 12.5f});
    antQuad.setPosition(mess.mPosition);
    antQuad.setHFlip(mess.mGoingRight);

    mAntSprites.emplace(mess.mAntId, AntSprite(antQuad, mess.mAntType));  
}

void Scene::handleMessage(const AntDeletionMessage& mess)
{
    mAntSprites.erase(mess.mAntId);
}

void Scene::handleMessage(const AntPositionMessage& mess)
{
    mAntSprites.at(mess.mAntId).quad.setPosition(mess.mOriginPosition);
    mAntSprites.at(mess.mAntId).quad.setRotation(mess.mAngle);
}

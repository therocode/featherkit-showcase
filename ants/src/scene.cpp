#include "scene.hpp"

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
    texture.create({width, height}, mLoader.loadImage(path).data(), smooth, interactive);
    mTextures.emplace(name, std::move(texture));
}

void Scene::createTextures()
{
    createTexture("dirt", "ants/data/textures/dirt.png", 800, 600, true, true);
    createTexture("dirtbg", "ants/data/textures/dirtbg.png", 800, 600, false);
    createTexture("backhills", "ants/data/textures/backhills.png", 800, 600, false);
    createTexture("fronthills", "ants/data/textures/fronthills.png", 800, 600, false);
    createTexture("cloud1", "ants/data/textures/cloud1.png", 243, 205);
    createTexture("cloud2", "ants/data/textures/cloud2.png", 265, 230);
    createTexture("cloud3", "ants/data/textures/cloud3.png", 302, 232);
    createTexture("cloud4", "ants/data/textures/cloud4.png", 312, 238);
    createTexture("ant", "ants/data/textures/ant.png", 184, 185);
    createTexture("darkness", "ants/data/textures/darkness.png", 800, 300);
    createTexture("halo", "ants/data/textures/halo.png", 145, 145);
}

void Scene::setupQuads()
{
    fea::Quad dirtQuad = fea::Quad({1600, 1200});
    fea::Quad dirtBgQuad = fea::Quad({1600, 1200});
    fea::Quad frontHillsQuad = fea::Quad({1700, 1200});
    fea::Quad backHillsQuad = fea::Quad({2240, 1200});

    dirtQuad.setTexture(mTextures.at("dirt"));
    dirtBgQuad.setTexture(mTextures.at("dirtbg"));
    frontHillsQuad.setTexture(mTextures.at("fronthills"));
    backHillsQuad.setTexture(mTextures.at("backhills"));

    frontHillsQuad.setParallax({0.7f, 1.0f});
    backHillsQuad.setParallax({0.5f, 1.0f});
    frontHillsQuad.setPosition({-300.0f, 0.0f});
    backHillsQuad.setPosition({-800.0f, 0.0f});

    mLandscapeQuads.push_back(backHillsQuad);
    mLandscapeQuads.push_back(frontHillsQuad);
    mLandscapeQuads.push_back(dirtBgQuad);
    mLandscapeQuads.push_back(dirtQuad);

    fea::Quad cloud1Quad = fea::Quad({243, 205});
    fea::Quad cloud2Quad = fea::Quad({265, 230});
    fea::Quad cloud3Quad = fea::Quad({302, 232});
    fea::Quad cloud4Quad = fea::Quad({312, 238});
    cloud1Quad.setTexture(mTextures.at("cloud1"));
    cloud2Quad.setTexture(mTextures.at("cloud2"));
    cloud3Quad.setTexture(mTextures.at("cloud3"));
    cloud4Quad.setTexture(mTextures.at("cloud4"));
    mCloudHandler.addCloud(cloud1Quad);
    mCloudHandler.addCloud(cloud2Quad);
    mCloudHandler.addCloud(cloud3Quad);
    mCloudHandler.addCloud(cloud4Quad);

    mLightingTarget.create({1600, 600});
    fea::Quad mLightingQuad = fea::Quad({1600, 600});
    fea::Quad mDarknessQuad = fea::Quad({1600, 600});
    fea::Quad mLargeHalo = fea::Quad({290, 290});
    fea::Quad mSmallHalo = fea::Quad({200, 200});

    mLightingQuad.setPosition({0, 600});
    mLightingQuad.setVFlip(true);
    mLightingQuad.setTexture(mLightingTarget.getTexture());
    mLargeHalo.setTexture(mTextures.at("halo"));
    mSmallHalo.setTexture(mTextures.at("halo"));

    mLightingQuads.emplace(LightingQuadType::LIGHT, mLightingQuad);
    mLightingQuads.emplace(LightingQuadType::DARK, mDarknessQuad);
    mLightingQuads.emplace(LightingQuadType::LARGE_HALO, mLargeHalo);
    mLightingQuads.emplace(LightingQuadType::SMALL_HALO, mSmallHalo);
}

void Scene::createAnimations()
{
    mAnimations.emplace((int)AntType::NORMAL,  fea::Animation(glm::vec2(0.0f, 37.0f * 0.0f), glm::vec2(46.0f, 37.0f), 4, 16));
    mAnimations.emplace((int)AntType::DIGGING, fea::Animation(glm::vec2(0.0f, 37.0f * 1.0f), glm::vec2(46.0f, 37.0f), 4,  8));
    mAnimations.emplace((int)AntType::BLUE,    fea::Animation(glm::vec2(0.0f, 37.0f * 2.0f), glm::vec2(46.0f, 37.0f), 4, 16));
    mAnimations.emplace((int)AntType::GREEN,   fea::Animation(glm::vec2(0.0f, 37.0f * 3.0f), glm::vec2(46.0f, 37.0f), 4, 16));
    mAnimations.emplace((int)AntType::RED,     fea::Animation(glm::vec2(0.0f, 37.0f * 4.0f), glm::vec2(46.0f, 37.0f), 4, 16));
}

void Scene::handleMessage(const AntCreationMessage& mess)
{
    fea::AnimatedQuad antQuad = fea::AnimatedQuad({46.0f, 37.0f});
    antQuad.setTexture(mTextures.at("ant")); 
    antQuad.setAnimation(mAnimations.at(mess.mAntType));
    antQuad.setOrigin(antQuad.getSize() / 2.0f);
    antQuad.setPosition(mess.mPosition);
    antQuad.setHFlip(!mess.mGoingRight);

    mAntSprites.emplace(mess.mAntId, AntSprite(antQuad, mess.mAntType));  
}

void Scene::handleMessage(const AntDeletionMessage& mess)
{
    mAntSprites.erase(mess.mAntId);
}

void Scene::handleMessage(const AntPositionMessage& mess)
{
    mAntSprites.at(mess.mAntId).mQuad.setPosition(mess.mOriginPosition);
    mAntSprites.at(mess.mAntId).mQuad.setRotation(mess.mAngle);
}

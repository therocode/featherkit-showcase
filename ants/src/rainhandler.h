#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <featherkit/render2d.h>

struct Rainsplash
{
    glm::vec2 mPos;
    glm::vec2 mVel;
    uint32_t ttl;
};

class RainHandler
{
    public:
        void update(const fea::Texture& solidMask);
        std::vector<glm::vec2> getRaindropPositions() const;
        std::vector<glm::vec2> getRainsplashPositions() const;
        std::vector<float> getRainsplashSizes() const;
        std::vector<glm::vec2> getCollisions() const;
    private:
        std::vector<glm::vec2> mRaindrops;
        std::vector<Rainsplash> mRainsplashes;
        std::vector<glm::vec2> mCollisions;
};

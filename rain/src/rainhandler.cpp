#include "rainhandler.h"
#include <cstdlib>
#include <iostream>

void RainHandler::update(const fea::Texture& solidMask)
{
    mCollisions.clear();

    //move current drops
    for(auto& drop : mRaindrops)
    {
        drop.x -= 6.0f;
        drop.y += 6.0f;
    }

    //create new drops
    if(!(rand() % 3))
        mRaindrops.push_back(glm::vec2(rand() % 3000, -20));

    //check for border removal and collisions
    for(auto iterator = mRaindrops.begin(); iterator != mRaindrops.end();)
    {
        if(iterator->x < -20)
        {
            iterator = mRaindrops.erase(iterator);
            continue;
        }
        
        uint32_t pixelX = std::max(0, std::min((int32_t)(iterator->x + 10.0f), 1600)) / 2;
        uint32_t pixelY = std::max(0, std::min((int32_t)(iterator->y + 18.0f), 1200)) / 2;
        
        if(solidMask.getPixel(pixelX, pixelY).a != 0.0f)
        {
            iterator = mRaindrops.erase(iterator);

            if(pixelX > 0 && pixelX < 800 && pixelY > 0 && pixelY < 600)
            {
                mCollisions.push_back(glm::vec2(pixelX, pixelY));

                for(uint32_t i = 0; i < 5; i++)
                {
                    Rainsplash newSplash({glm::vec2(pixelX * 2 - 6, pixelY * 2 - 10), glm::vec2(((float)(rand() % 21) / 4.0f) - 2.5f, ((rand() % 21) / 4.0f) - 2.5f), static_cast<uint32_t>((rand() % 60) + 40)});
                    mRainsplashes.push_back(newSplash);
                }
            }
            continue;
        }

        iterator++;
    }

    //animate splashes
    for(auto iterator = mRainsplashes.begin(); iterator != mRainsplashes.end();)
    {
        iterator->ttl -= 5;

        if(iterator->ttl > 100)
        {
            iterator = mRainsplashes.erase(iterator);
            continue;
        }

        iterator->mPos += iterator->mVel;

        iterator++;
    }
}

std::vector<glm::vec2> RainHandler::getRaindropPositions() const
{
    return mRaindrops;
}

std::vector<glm::vec2> RainHandler::getRainsplashPositions() const
{
    std::vector<glm::vec2> result;

    for(auto& splash : mRainsplashes)
        result.push_back(splash.mPos);

    return result;
}

std::vector<float> RainHandler::getRainsplashSizes() const
{
    std::vector<float> result;

    for(auto& splash : mRainsplashes)
        result.push_back((float)splash.ttl / 100.0f);

    return result;
}

std::vector<glm::vec2> RainHandler::getCollisions() const
{
    return mCollisions;
}

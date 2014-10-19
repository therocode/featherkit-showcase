#include "cloudhandler.h"

CloudHandler::CloudHandler()
{
    cloudVelocity = glm::vec2(1.0f, 0.0f);
}

void CloudHandler::update()
{
    for(auto& cloud : mClouds)
    {
        cloud.setPosition(cloud.getPosition() + cloudVelocity);
        if(cloud.getPosition().x > 1650.0f)
        {
            cloud.setPosition({-270.0f, cloud.getPosition().y});
        }
    }
}

void CloudHandler::addCloud(fea::Quad cloud)
{
    std::vector<glm::vec2> cloudPositions = {{   0.0f, 100.0f},
                                             { 500.0f, 230.0f},
                                             {1000.0f,  60.0f},
                                             {1500.0f, 250.0f}};

    size_t numberOfCloudsAdded = mClouds.size();
    cloud.setPosition(cloudPositions.at(numberOfCloudsAdded));
    mClouds.push_back(cloud);
}

const std::vector<fea::Quad>& CloudHandler::getCloudQuads()
{
    return mClouds;
}

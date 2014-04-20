#include "cloudhandler.h"

CloudHandler::CloudHandler()
{
    cloudPositions.push_back({0.0f, 100.0f});
    cloudPositions.push_back({500.0f, 230.0f});
    cloudPositions.push_back({1000.0f, 60.0f});
    cloudPositions.push_back({1500.0f, 250.0f});

    cloudVelocity = glm::vec2(1.0f, 0.0f);
}

void CloudHandler::update()
{
    for(auto& position : cloudPositions)
    {
        position += cloudVelocity;
        if(position.x > 1650.0f)
            position.x = -270.0f;
    }
}

std::vector<glm::vec2> CloudHandler::getCloudPositions()
{
    return cloudPositions;
}

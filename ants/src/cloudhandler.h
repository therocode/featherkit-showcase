#pragma once
#include <vector>
#include <glm/glm.hpp>

class CloudHandler
{
    public:
        CloudHandler();
        void update();
        std::vector<glm::vec2> getCloudPositions();

    private:
        std::vector<glm::vec2> cloudPositions;
        glm::vec2 cloudVelocity;
};

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <fea/rendering/quad.hpp>

class CloudHandler
{
    public:
        CloudHandler();
        void update();
        void addCloud(fea::Quad cloud);
        const std::vector<fea::Quad>& getCloudQuads();

    private:
        std::vector<fea::Quad> mClouds;
        glm::vec2 cloudVelocity;
};

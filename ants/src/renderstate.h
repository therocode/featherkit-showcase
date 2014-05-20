#pragma once
#include <glm/glm.hpp>

struct RenderState
{
    RenderState(glm::vec2 cameraPos, float cameraZoom)
    {
        cameraPosition = cameraPos;
        cameraZoomTarget = cameraZoom;
    }
    glm::vec2 cameraPosition;
    float cameraZoomTarget;
};

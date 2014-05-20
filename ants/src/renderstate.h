#pragma once
#include <glm/glm.hpp>

struct RenderState
{
    RenderState(glm::vec2 cameraPos, float cameraZoom, glm::vec2 topLeftCameraBound, glm::vec2 bottomRightCameraBound)
    {
        cameraPosition = cameraPos;
        cameraZoomTarget = cameraZoom;
        topLeftCameraBoundary = topLeftCameraBound;
        bottomRightCameraBoundary = bottomRightCameraBound;
    }
    glm::vec2 cameraPosition;
    float cameraZoomTarget;
    glm::vec2 topLeftCameraBoundary;
    glm::vec2 bottomRightCameraBoundary;
};

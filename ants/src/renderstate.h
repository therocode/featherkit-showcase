#pragma once
#include <glm/glm.hpp>

struct RenderState
{
    RenderState(float cameraZoom, glm::vec2 topLeftCameraBound, glm::vec2 bottomRightCameraBound)
    {
        cameraZoomTarget = cameraZoom;
        topLeftCameraBoundary = topLeftCameraBound;
        bottomRightCameraBoundary = bottomRightCameraBound;
    }
    float cameraZoomTarget;
    glm::vec2 topLeftCameraBoundary;
    glm::vec2 bottomRightCameraBoundary;
};

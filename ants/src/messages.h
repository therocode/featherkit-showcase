#pragma once
#include <glm/glm.hpp>
#include <featherkit/messaging.h>

FEA_DECLARE_MESSAGE(QuitMessage);
FEA_DECLARE_MESSAGE(CameraPositionMessage, const glm::vec2&);       // vel to add
FEA_DECLARE_MESSAGE(AntPositionMessage, const glm::vec2&, float);    // origin position, angle

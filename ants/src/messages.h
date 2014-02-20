#pragma once
#include <glm/glm.hpp>
#include <featherkit/messaging.h>

FEA_DECLARE_MESSAGE(QuitMessage);
FEA_DECLARE_MESSAGE(CameraPositionMessage, const glm::vec2&);       // vel to add
FEA_DECLARE_MESSAGE(AntPositionMessage, int, const glm::vec2&, float);    // vector index, origin position, angle
FEA_DECLARE_MESSAGE(AntPointsMessage, const glm::vec2&, const glm::vec2&);
FEA_DECLARE_MESSAGE(AntQuadCreationMessage, const glm::vec2&);

#pragma once
#include <glm/glm.hpp>
#include <featherkit/messaging.h>
#include <featherkit/rendering/texture.h>

FEA_DECLARE_MESSAGE(QuitMessage);
FEA_DECLARE_MESSAGE(CameraPositionMessage, const glm::vec2&);       // vel to add
FEA_DECLARE_MESSAGE(AntPositionMessage, size_t, const glm::vec2&, float);    // ant id, origin position, angle
FEA_DECLARE_MESSAGE(AntPointsMessage, const glm::vec2&, const glm::vec2&);
FEA_DECLARE_MESSAGE(AntCreationMessage, size_t, bool, bool, const glm::vec2&, float);  // ant id, type(digging), goingRight, position, velocity
FEA_DECLARE_MESSAGE(AntOutsideBoundariesMessage, int) // ant id
FEA_DECLARE_MESSAGE(AntDeletionMessage, int) // ant id
FEA_DECLARE_MESSAGE(DiggerAntCreatedMessage, size_t); // digger ant id
FEA_DECLARE_MESSAGE(AntStartedDiggingMessage, bool); // A and not B
FEA_DECLARE_MESSAGE(MouseClickMessage, glm::vec2);

FEA_DECLARE_MESSAGE(DirtTextureSetMessage, const fea::Texture*);    // dirt texture

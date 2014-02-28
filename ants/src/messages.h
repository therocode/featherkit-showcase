#pragma once
#include <glm/glm.hpp>
#include <featherkit/messaging.h>
#include <featherkit/rendering/texture.h>

FEA_DECLARE_MESSAGE(QuitMessage);
FEA_DECLARE_MESSAGE(CameraPositionMessage, const glm::vec2&);       // vel to add
FEA_DECLARE_MESSAGE(AntPositionMessage, int, const glm::vec2&, float);    // vector index, origin position, angle
FEA_DECLARE_MESSAGE(AntPointsMessage, const glm::vec2&, const glm::vec2&);
FEA_DECLARE_MESSAGE(AntCreationMessage, bool, bool, const glm::vec2&);  // type(digging), goingRight, position
FEA_DECLARE_MESSAGE(AntOutsideBoundariesMessage, int) // vector index
FEA_DECLARE_MESSAGE(AntDeletionMessage, int) // vector index

FEA_DECLARE_MESSAGE(DirtTextureSetMessage, const fea::Texture*);    // dirt texture

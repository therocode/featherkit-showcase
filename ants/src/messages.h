#pragma once
#include "anttype.h"
#include <glm/glm.hpp>
#include <fea/messaging.hpp>
#include <fea/rendering/texture.hpp>

FEA_DECLARE_MESSAGE(QuitMessage);
FEA_DECLARE_MESSAGE(CameraPositionMessage, const glm::vec2&);       // vel to add

FEA_DECLARE_MESSAGE(AntPositionMessage, size_t, const glm::vec2&, float);    // ant id, origin position, angle
FEA_DECLARE_MESSAGE(AntCreationMessage, size_t, AntType, bool, const glm::vec2&, float);  // ant id, goingRight, position, velocity
FEA_DECLARE_MESSAGE(AntDeletionMessage, int) // ant id
FEA_DECLARE_MESSAGE(AntOutsideBoundariesMessage, int) // ant id
FEA_DECLARE_MESSAGE(DiggerAntCreatedMessage, size_t); // digger ant id
FEA_DECLARE_MESSAGE(AntStartedDiggingMessage, size_t, glm::vec2); // position of digging target, digger ant id
FEA_DECLARE_MESSAGE(AntStoppedDiggingMessage, size_t); // position of digging target, digger ant id

FEA_DECLARE_MESSAGE(MouseClickMessage, glm::vec2); // mouse position of the click
FEA_DECLARE_MESSAGE(MousePositionMessage, glm::vec2); // mouse position in window coords

FEA_DECLARE_MESSAGE(DirtTextureSetMessage, fea::Texture*);    // dirt texture

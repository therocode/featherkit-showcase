#pragma once
#include "anttype.hpp"
#include "buttontype.hpp"
#include <glm/glm.hpp>
#include <fea/util.hpp>
#include <fea/rendering/texture.hpp>

struct QuitMessage
{
};

struct CameraPositionMessage
{
    const glm::vec2& mVelocityToAdd;
};       

struct AntPositionMessage
{
    size_t mAntId;
    const glm::vec2& mOriginPosition;
    float mAngle;
};

struct AntCreationMessage
{
    size_t mAntId;
    AntType mAntType;
    bool mGoingRight;
    const glm::vec2& mPosition; 
    float mVelocity;
};

struct AntDeletionMessage
{
    size_t mAntId;
};

struct AntOutsideBoundariesMessage
{
    size_t mAntId;
};

struct DiggerAntCreatedMessage
{
    size_t mDiggerAntId;
};

struct AntStartedDiggingMessage
{
    size_t mDiggerAntId;
    glm::vec2 mDiggingTargetPosition;
};

struct AntStoppedDiggingMessage
{
    size_t mDiggerAntId;
};

struct MouseClickMessage
{
};

struct MousePositionMessage
{
    glm::vec2 mMousePosition; // in window coords
};

struct DirtTextureSetMessage
{
    fea::Texture* mDirtTexture;
};

struct GuiButtonClickedMessage
{
    ButtonType mButtonType;
};

#pragma once
#include <glm/glm.hpp>
#include <featherkit/messaging.h>

struct QuitMsg_tag{};
using QuitMessage = fea::Message<QuitMsg_tag>;

struct CamPosMsg_tag{}; //                                vel to add
using CameraPositionMessage = fea::Message<CamPosMsg_tag, glm::vec2>;

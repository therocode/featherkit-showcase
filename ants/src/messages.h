#pragma once
#include <featherkit/messaging.h>

struct QuitMsg_tag{};
using QuitMessage = fea::Message<QuitMsg_tag>;

#pragma once
#include "anttype.h"
#include <featherkit/rendering/animatedquad.hpp>

struct AntSprite
{
    AntSprite(fea::AnimatedQuad q, AntType t)
    {
        quad = q;
        type = t;
    }
    fea::AnimatedQuad quad;
    AntType type;
};

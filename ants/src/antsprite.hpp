#pragma once
#include "anttype.hpp"
#include <fea/rendering/animatedquad.hpp>

struct AntSprite
{
    AntSprite(fea::AnimatedQuad quad, AntType type)
    {
        mQuad = quad;
        mType = type;
    }
    fea::AnimatedQuad mQuad;
    AntType mType;
};

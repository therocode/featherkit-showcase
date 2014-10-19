#include "wiggletext.h"
#include <iostream>

WiggleText::WiggleText()
{
    mRotationVelocity = 0.002f;
    mScalingVelocity = 0.999f;
    mTextFont = fea::Font("ants/data/fonts/Champagne_Limousines_Bold.ttf", 50);
    mTextString = "'Ants'\nWritten in Feather Kit\nBy @kimspindel";
}

void WiggleText::setup()
{
    mTextSurface.setParallax({0.0f, 0.0f});
    mTextSurface.setPenFont(mTextFont);
    mTextSurface.setColor(fea::Color::Black);
    mTextSurface.setLineHeight(50.0f);
    mTextSurface.write(mTextString);
    mTextSurface.setOrigin(mTextSurface.getSize()/2.0f);
    mTextSurface.setPosition({50.0f, 0.0f});

    mDrawables.push_back(&mTextSurface);
}

void WiggleText::wiggle()
{
    mTextSurface.rotate(mRotationVelocity);  // make this "animated" :D
    if((mTextSurface.getRotation() < -0.10f) ||
       (mTextSurface.getRotation() > 0.10f))
    {
        mRotationVelocity = -mRotationVelocity;
    }
    mTextSurface.scale({mScalingVelocity, mScalingVelocity});  // make this "animated" :D
    if(mTextSurface.getScale().x < 0.98f)
    {
        mScalingVelocity = 1.001f;
    }
    else if(mTextSurface.getScale().x > 1.02f)
    {
        mScalingVelocity = 0.999f;
    }
}

std::vector<fea::Drawable2D*> WiggleText::getDrawables()
{
    return mDrawables;
}

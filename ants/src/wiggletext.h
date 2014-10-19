#pragma once
#include <fea/render2d.hpp>
#include <fea/rendering/textsurface.hpp>

class WiggleText
{
    public:
        WiggleText();
        void wiggle();
        void setup();
        std::vector<fea::Drawable2D*> getDrawables();

    private:
        std::vector<fea::Drawable2D*> mDrawables;

        fea::TextSurface mTextSurface;
        std::string mTextString{"hej\n"};
        fea::Font mTextFont;
        float mRotationVelocity;
        float mScalingVelocity;
};

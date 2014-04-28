#pragma once
#include "featurebutton.h"
#include <featherkit/render2d.hpp>

class GUI
{
    public:
        GUI();
        std::vector<fea::Drawable2D*> getDrawables();
        void setup();

    private:
        std::vector<fea::Drawable2D*> drawables;
        std::vector<FeatureButton> featureButtons;
        fea::Quad backgroundQuad;
};

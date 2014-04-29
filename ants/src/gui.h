#pragma once
#include "messages.h"
#include "featurebutton.h"
#include <featherkit/render2d.hpp>

class GUI
    :   public fea::MessageReceiver<MousePositionMessage>,
        public fea::MessageReceiver<MouseClickMessage>
{
    public:
        GUI();
        std::vector<fea::Drawable2D*> getDrawables();
        void setup();

        virtual void handleMessage(const MousePositionMessage& mess) override;
        virtual void handleMessage(const MouseClickMessage& mess) override;

    private:
        std::vector<fea::Drawable2D*> drawables;
        std::vector<FeatureButton> featureButtons;
        fea::Quad backgroundQuad;
        glm::vec2 origin;

        FeatureButton btn;
};

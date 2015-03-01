#pragma once
#include "messages.hpp"
#include "featurebutton.hpp"
#include <fea/render2d.hpp>

class GUI
    :   public fea::MessageReceiver<MousePositionMessage, MouseClickMessage>
{
    public:
        GUI(fea::MessageBus& b);
        std::vector<fea::Drawable2D*> getDrawables();
        void setup();
        void update();

        virtual void handleMessage(const MousePositionMessage& mess) override;
        virtual void handleMessage(const MouseClickMessage& mess) override;

    private:
        fea::MessageBus& bus;
        std::vector<fea::Drawable2D*> drawables;
        std::vector<std::unique_ptr<FeatureButton>> featureButtons;
        fea::Quad backgroundQuad;
        glm::vec2 origin;
        glm::vec2 buttonSize;
};

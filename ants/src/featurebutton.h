#pragma once
#include <glm/glm.hpp>
#include <fea/render2d.hpp>
#include <fea/rendering/textsurface.hpp>
#include "buttontype.h"

enum ButtonState
{
    CLOSED = 0, CLOSING, OPENING, OPENED
};

class FeatureButton
{
    public:
        FeatureButton(glm::vec2 pos, glm::vec2 size, std::string title, std::string content, ButtonType type);
        float getLength(); 
        void setPosition(glm::vec2 pos);
        bool withinArea(glm::vec2 pos);
        void setClicked(bool click);
        void setHovered(bool hover);
        bool isHovered();
        bool isOpening();
        ButtonType getButtonType();

        std::vector<fea::Drawable2D*> getDrawables();
        void update(float lengthUpdate);

    private:
        ButtonState state;
        ButtonType buttonType;
        glm::vec2 position;
        float padding;

        fea::Color offColour;
        fea::Color onColour;

        fea::Quad largeQuad;
        fea::Quad smallQuad;
        float originalLength;
        float expandedLength;
        fea::TextSurface titleSurface;
        fea::TextSurface contentSurface;
        std::string titleText;
        std::string contentText;
        fea::Font titleFont;
        fea::Font contentFont;
        std::vector<fea::Drawable2D*> drawables;

        bool clicked;
        bool hovered;

        float slideVelocity;
};

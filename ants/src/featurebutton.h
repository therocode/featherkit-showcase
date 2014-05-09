#pragma once
#include <glm/glm.hpp>
#include <featherkit/render2d.hpp>
#include <featherkit/rendering/textsurface.hpp>

enum ButtonState
{
    CLOSED = 0, CLOSING, OPENING, OPENED
};

class FeatureButton
{
    public:
        FeatureButton(glm::vec2 pos, glm::vec2 size, std::string title, std::string content);
        float getLength(); 
        void setPosition(glm::vec2 pos);
        bool withinArea(glm::vec2 pos);
        void setClicked(bool click);
        void setHovered(bool hover);
        bool isHovered();
        ButtonState getState();

        std::vector<fea::Drawable2D*> getDrawables();
        void update(float lengthUpdate);

    private:
        ButtonState state;
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

        bool clicked;
        bool hovered;

        float targetLength;
        float slideVelocity;
};

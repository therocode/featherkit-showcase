#include "featurebutton.h"

FeatureButton::FeatureButton(glm::vec2 pos, glm::vec2 size, std::string title)
{
    padding = 5.0f;

    position = pos;
    largeQuad = fea::Quad(size);
    largeQuad.setPosition(position);
    largeQuad.setParallax({0, 0});
    largeQuad.setColor(fea::Color(10, 10, 10, 123));

    smallQuad = fea::Quad(size - glm::vec2(padding * 2.0f, padding * 2.0f));
    smallQuad.setPosition(position + glm::vec2(padding, padding));
    smallQuad.setParallax({0, 0});
    smallQuad.setColor(fea::Color(240, 240, 240, 123));

    titleText = title;
}

/*
        glm::vec2 position;
        float padding;
        fea::Quad largeQuad;
        fea::Quad smallQuad;
        fea::TextSurface titleSurface;
        fea::TextSurface contentSurface;
        std::string titleText;
        std::string contentText;
        bool clicked;
        bool hovered;
        */

const glm::vec2& FeatureButton::getPosition()
{
    return position;
}

void FeatureButton::setPosition(glm::vec2 pos)
{
    position = pos;
}

bool FeatureButton::withinPosition(glm::vec2 pos)
{
}

void FeatureButton::setClicked(bool click)
{
    clicked = click;
}

void FeatureButton::setHovered(bool hover)
{
    hovered = hover;
}

bool FeatureButton::isHovered()
{
    return hovered;
}

std::vector<fea::Drawable2D*> FeatureButton::getDrawables()
{
    std::vector<fea::Drawable2D*> drawables;
    drawables.push_back(&largeQuad);
    drawables.push_back(&smallQuad);

    return drawables;
}

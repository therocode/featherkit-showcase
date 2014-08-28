#include "featurebutton.h"
#include <iostream>

FeatureButton::FeatureButton(glm::vec2 pos, glm::vec2 size, std::string title, std::string content, ButtonType type)
{
    padding = 5.0f;

    offColour = fea::Color(240, 240, 240, 123);
    onColour = fea::Color(100, 100, 100, 123);

    position = pos;
    largeQuad = fea::Quad(size);
    largeQuad.setPosition(position);
    largeQuad.setParallax({0.0f, 0.0f});
    largeQuad.setColor(fea::Color(10, 10, 10, 123));

    smallQuad = fea::Quad(size - glm::vec2(padding * 2.0f, padding * 2.0f));
    smallQuad.setPosition(position + glm::vec2(padding, padding));
    smallQuad.setParallax({0.0f, 0.0f});
    smallQuad.setColor(offColour);

    contentQuad = fea::Quad(size - glm::vec2(padding * 2.0f, padding * 2.0f));
    contentQuad.setPosition(position + glm::vec2(padding, padding) + glm::vec2(0.0f, padding + smallQuad.getSize().y));
    contentQuad.setParallax({0.0f, 0.0f});
    contentQuad.setColor(offColour);

    titleFont = fea::Font("ants/data/fonts/Champagne_Limousines_Bold.ttf", 23);
    titleText = title;                                                  
    titleSurface.setParallax({0.0f, 0.0f});
    titleSurface.setPenFont(titleFont);
    titleSurface.setColor(fea::Color(255, 255, 255));
    titleSurface.write(titleText);
    titleSurface.setPosition(position + glm::vec2(padding, padding) + glm::vec2(5.0f, titleSurface.getSize().y));

    contentFont = fea::Font("ants/data/fonts/Champagne_Limousines.ttf", 18);
    contentText = content;
    contentSurface.setParallax({0.0f, 0.0f});
    contentSurface.setPenFont(contentFont);
    contentSurface.setColor(fea::Color(255, 255, 255, 0));
    contentSurface.enableWordWrap(true);
    contentSurface.setLineWidth(smallQuad.getSize().x - (2.0f * padding));
    contentSurface.setLineHeight(18.0f);
    contentSurface.write(contentText);
    contentSurface.setPosition(position + glm::vec2(padding, padding) + glm::vec2(5.0f, padding + titleSurface.getSize().y + smallQuad.getSize().y));

    contentQuad.setSize(glm::vec2(contentQuad.getSize().x, contentSurface.getSize().y + (2.0f * padding)));

    originalLength = size.y;
    //std::cout << "content surface length is: " << contentSurface.getSize().y << "\n";
    expandedLength = smallQuad.getSize().y + contentSurface.getSize().y + (4.0f * padding);
    slideVelocity = 1.0f;

    state = ButtonState::CLOSED;
    buttonType = type;

    drawables.push_back(&largeQuad);
    drawables.push_back(&smallQuad);
    drawables.push_back(&contentQuad);
    drawables.push_back(&titleSurface);
    drawables.push_back(&contentSurface);
}

float FeatureButton::getLength()
{
    return largeQuad.getSize().y;
}

void FeatureButton::setPosition(glm::vec2 pos)
{
    position = pos;
}

bool FeatureButton::withinArea(glm::vec2 pos)
{
    glm::vec2 boundaryPosition = position - glm::vec2(-400.0f, -300.0f);
    glm::vec2 boundaryPosition2 = boundaryPosition + glm::vec2(largeQuad.getSize().x, originalLength);
    return((pos.x >= boundaryPosition.x) &&
           (pos.x <= boundaryPosition2.x) &&
           (pos.y >= boundaryPosition.y) &&
           (pos.y <= boundaryPosition2.y));
}

void FeatureButton::setClicked(bool click)
{
    clicked = click;
    if(clicked)
    {
        if(state == ButtonState::OPENING || state == ButtonState::OPENED)
            state = ButtonState::CLOSING;
        else if(state == ButtonState::CLOSING || state == ButtonState::CLOSED)
            state = ButtonState::OPENING;
    }
    else
    {
        // closing on other clicks means
        // only one button opens at a time
        state = ButtonState::CLOSING;
    }
}

void FeatureButton::setHovered(bool hover)
{
    hovered = hover;
    smallQuad.setColor(hovered ? onColour : offColour);
}

bool FeatureButton::isHovered()
{
    return hovered;
}

bool FeatureButton::isOpening()
{
    return (state == ButtonState::OPENING);
}

ButtonType FeatureButton::getButtonType()
{
    return buttonType;
}

std::vector<fea::Drawable2D*> FeatureButton::getDrawables()
{
    return drawables;
}

void FeatureButton::update(float lengthUpdate)
{
    // update position
    position = glm::vec2(position.x, lengthUpdate);
    largeQuad.setPosition(position);
    smallQuad.setPosition(position + glm::vec2(padding, padding));
    titleSurface.setPosition(position + glm::vec2(padding, padding) + glm::vec2(5.0f, titleSurface.getSize().y + 10.0f));

    // update size
    switch(state)
    {
        case ButtonState::CLOSED:
            // do nothing, I think
            break;
        case ButtonState::CLOSING:
        {
            contentSurface.setColor(fea::Color(255, 255, 255, 0));
            contentQuad.setColor(fea::Color(255, 255, 255, 0));
            glm::vec2 quadSize = largeQuad.getSize();

            if(quadSize.y > originalLength)
            {
                largeQuad.setSize({quadSize.x, quadSize.y - slideVelocity});
            }
            else
            {
                largeQuad.setSize({quadSize.x, originalLength});
                state = ButtonState::CLOSED;
            }
            break;
        }
        case ButtonState::OPENING:
        {
            glm::vec2 quadSize = largeQuad.getSize();

            if(quadSize.y < expandedLength)
            {
                largeQuad.setSize({quadSize.x, quadSize.y + slideVelocity});
            }
            else
            {
                largeQuad.setSize({quadSize.x, expandedLength});
                state = ButtonState::OPENED;
                contentSurface.setColor(fea::Color(255, 255, 255, 255));
                contentQuad.setColor(fea::Color(255, 255, 255, 255));
            }
            break;
        }
        case ButtonState::OPENED:
            // also do nothing
            break;
    }
}

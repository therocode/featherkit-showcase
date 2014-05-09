#include "featurebutton.h"
#include <iostream>

FeatureButton::FeatureButton(glm::vec2 pos, glm::vec2 size, std::string title, std::string content)
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

    titleFont = fea::Font("ants/data/fonts/Champagne_Limousines_Bold.ttf", 40);
    titleText = title;                                                  
    titleSurface.setParallax({0.0f, 0.0f});
    titleSurface.setPenFont(titleFont);
    titleSurface.setColor(fea::Color(255, 255, 255));
    titleSurface.write(titleText);
    titleSurface.setPosition(position + glm::vec2(padding, padding) + glm::vec2(5.0f, titleSurface.getSize().y));

    contentFont = fea::Font("ants/data/fonts/Champagne_Limousines.ttf", 30);
    contentText = content;
    contentSurface.setParallax({0.0f, 0.0f});
    contentSurface.setPenFont(contentFont);
    contentSurface.setColor(fea::Color(255, 255, 255));
    contentSurface.write(contentText);

    originalLength = size.y;
    //expandedLength = contentSurface.getSize().y;
    expandedLength = 62.0f;
    slideVelocity = 1.0f;

    state = ButtonState::CLOSED;
}

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
    if(clicked)
    {
        if(state == ButtonState::OPENING || state == ButtonState::OPENED)
            state = ButtonState::CLOSING;
        else if(state == ButtonState::CLOSING || state == ButtonState::CLOSED)
            state = ButtonState::OPENING;
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

ButtonState FeatureButton::getState()
{
    return state;
}

std::vector<fea::Drawable2D*> FeatureButton::getDrawables()
{
    std::vector<fea::Drawable2D*> drawables;
    drawables.push_back(&largeQuad);
    drawables.push_back(&smallQuad);
    drawables.push_back(&titleSurface);

    return drawables;
}

void FeatureButton::update()
{
    switch(state)
    {
        case ButtonState::CLOSED:
            // do nothing, I think
            //std::cout << "test\n";
            break;
        case ButtonState::CLOSING:
        {
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
            //std::cout << "\n///////////////I'm trying\n";
            glm::vec2 quadSize = largeQuad.getSize();
            if(quadSize.y < expandedLength)
            {
                largeQuad.setSize({quadSize.x, quadSize.y + slideVelocity});
            }
            else
            {
                largeQuad.setSize({quadSize.x, expandedLength});
                state = ButtonState::OPENED;
            }
            break;
        }
        case ButtonState::OPENED:
            // show text
            std::cout << "hi it is open\n";
            break;
    }
}

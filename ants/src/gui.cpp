#include "gui.h"

GUI::GUI()
{
}

void GUI::setup()
{
    origin = glm::vec2(-400.0f, -300.0f);
    buttonSize = glm::vec2(200.0f, 50.0f);

    backgroundQuad = fea::Quad({200.0f, 600.0f});
    backgroundQuad.setPosition(origin);
    backgroundQuad.setParallax({0.0f, 0.0f});
    backgroundQuad.setColor(fea::Color(123, 123, 123, 123));

    drawables.push_back(&backgroundQuad);

    for(size_t i = 0; i < 3; i++)
    {
        featureButtons.push_back(std::unique_ptr<FeatureButton>(new FeatureButton(glm::vec2(origin.x, origin.y + buttonSize.y * i), buttonSize, "hej",
        "lol bajs this is a description")));
    }

    for(auto& button : featureButtons)
    {
        for(auto& drawable : button->getDrawables())
        {
            drawables.push_back(drawable);
        }
    }
}

void GUI::update()
{
    for(auto& button : featureButtons)
    {
        button->update();
    }
}

std::vector<fea::Drawable2D*> GUI::getDrawables()
{
    return drawables;
}

void GUI::handleMessage(const MousePositionMessage& mess)
{
    // check for hovering
    glm::vec2 position;
    std::tie(position) = mess.mData;
    for(size_t i = 0; i < featureButtons.size(); i++)
    {
        featureButtons.at(i)->setHovered(withinArea(position, i));
    }
}

bool GUI::withinArea(glm::vec2 position, size_t index)
{
    glm::vec2 boundaryPosition = glm::vec2(0.0f, buttonSize.y * index);
    glm::vec2 boundaryPosition2 = boundaryPosition + buttonSize;
    return((position.x >= boundaryPosition.x) &&
           (position.x <= boundaryPosition2.x) &&
           (position.y >= boundaryPosition.y) &&
           (position.y <= boundaryPosition2.y));
}

void GUI::handleMessage(const MouseClickMessage& mess)
{
    // check for clicking
    glm::vec2 position;
    std::tie(position) = mess.mData;
    for(auto& button : featureButtons)
    {
        button->setClicked(button->isHovered());
    }
}

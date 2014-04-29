#include "gui.h"

GUI::GUI()
    :
    origin({-400.0f, -300.0f}),
    btn(origin, {200, 50}, "hej")
{
}

void GUI::setup()
{
    backgroundQuad = fea::Quad({200, 600});
    backgroundQuad.setPosition(origin);
    backgroundQuad.setParallax({0, 0});
    backgroundQuad.setColor(fea::Color(123, 123, 123, 123));

    drawables.push_back(&backgroundQuad);

    for(auto& drawable : btn.getDrawables())
    {
        drawables.push_back(drawable);
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
    /*
    for(auto& button : featureButtons)
    {
        button.setHovered(button.withinArea(position));
    }
    */
}

void GUI::handleMessage(const MouseClickMessage& mess)
{
    // check for clicking
    glm::vec2 position;
    std::tie(position) = mess.mData;
    /*
    for(auto& button : featureButtons)
    {
        button.setClicked(button.isHovered());
    }
    */
}

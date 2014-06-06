#include "gui.h"
#include "buttontype.h"

GUI::GUI(fea::MessageBus& b)
  : bus(b)
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
        "lol bajs this is a description", (ButtonType)i)));
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
    float yPos = origin.y;
    for(auto& button : featureButtons)
    {
        button->update(yPos);
        yPos += button->getLength();
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
        featureButtons.at(i)->setHovered(featureButtons.at(i)->withinArea(position));
    }
}

void GUI::handleMessage(const MouseClickMessage& mess)
{
    // check for clicking
    glm::vec2 position;
    std::tie(position) = mess.mData;
    bool noneOpen = true; // check to see if any are open

    for(auto& button : featureButtons)
    {
        button->setClicked(button->isHovered());
        if(button->isOpening())
        {   
            bus.send(GuiButtonClickedMessage(button->getButtonType()));
            noneOpen = false;
        }
    }

    if(noneOpen)
        bus.send(GuiButtonClickedMessage(ButtonType::B_DEFAULT));
}

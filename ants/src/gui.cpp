#include "gui.hpp"
#include "buttontype.hpp"

GUI::GUI(fea::MessageBus& b)
  : bus(b)
{
    fea::subscribe(bus, *this);
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

    featureButtons.push_back(std::unique_ptr<FeatureButton>(new FeatureButton(glm::vec2(origin.x, origin.y + buttonSize.y * 0), buttonSize,
        "Interactive textures",
        "Texture pixels can be read and modified easily. Physics and collision detection can be texture dependent.",
        ButtonType::B_INTERACTIVE)));
    featureButtons.push_back(std::unique_ptr<FeatureButton>(new FeatureButton(glm::vec2(origin.x, origin.y + buttonSize.y * 1), buttonSize,
        "Color blend modes",
        "Feather Kit provides a means to render drawables to textures as well as changing blend modes. This functionality can be used to create lighting effects.",
        ButtonType::B_COLOUR_BLEND)));
    featureButtons.push_back(std::unique_ptr<FeatureButton>(new FeatureButton(glm::vec2(origin.x, origin.y + buttonSize.y * 2), buttonSize, 
        "Parallax",
        "Feather Kit provides built-in parallax functions to give perspective effects. Try moving the camera left and right.",
        ButtonType::B_PARALLAX)));
    featureButtons.push_back(std::unique_ptr<FeatureButton>(new FeatureButton(glm::vec2(origin.x, origin.y + buttonSize.y * 3), buttonSize,
        "Animations",
        "Feather Kit has animated quads which make it easy to create animated sprites.",
        ButtonType::B_ANIMATION)));
    featureButtons.push_back(std::unique_ptr<FeatureButton>(new FeatureButton(glm::vec2(origin.x, origin.y + buttonSize.y * 4), buttonSize,
        "Text",
        "Featherkit has built-in functionality for rendering text.",
        ButtonType::B_TEXT)));
    featureButtons.push_back(std::unique_ptr<FeatureButton>(new FeatureButton(glm::vec2(origin.x, origin.y + buttonSize.y * 5), buttonSize,
        "Camera/viewport",
        "Featherkit's camera and viewport system make it easy to choose how the scene is rendered and provides support for multiple cameras and viewports.",
        ButtonType::B_CAMERA)));

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
    for(size_t i = 0; i < featureButtons.size(); i++)
    {
        featureButtons.at(i)->setHovered(featureButtons.at(i)->withinArea(mess.mMousePosition));
    }
}

void GUI::handleMessage(const MouseClickMessage& mess)
{
    (void)mess;
    // check for clicking
    bool noneOpen = true; // check to see if any are open

    for(auto& button : featureButtons)
    {
        button->setClicked(button->isHovered());
        if(button->isOpening())
        {   
            bus.send(GuiButtonClickedMessage({button->getButtonType()}));
            noneOpen = false;
        }
    }

    if(noneOpen)
        bus.send(GuiButtonClickedMessage({ButtonType::B_DEFAULT}));
}

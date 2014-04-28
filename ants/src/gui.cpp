#include "gui.h"

GUI::GUI()
{
}

void GUI::setup()
{
    backgroundQuad = fea::Quad({200, 600});
    backgroundQuad.setPosition({0, 0});
    backgroundQuad.setColor(fea::Color(123, 123, 123, 123));

    drawables.push_back(&backgroundQuad);
}

std::vector<fea::Drawable2D*> GUI::getDrawables()
{
    return drawables;
}

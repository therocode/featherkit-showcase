#pragma once

class GUI
{
    public:
        GUI();
        std::vector<fea::Drawable2D> getDrawables();

    private:
        std::vector<fea::Drawable2D> drawables;
};

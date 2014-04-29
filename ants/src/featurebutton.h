#pragma once
#include <glm/glm.hpp>
#include <featherkit/render2d.hpp>
#include <featherkit/rendering/textsurface.hpp>

class FeatureButton
{
    public:
        FeatureButton(glm::vec2 pos, glm::vec2 size, std::string title);
        const glm::vec2& getPosition(); // necessary?
        void setPosition(glm::vec2 pos);
        bool withinPosition(glm::vec2 pos);
        void setClicked(bool click);
        void setHovered(bool hover);
        bool isHovered();

        std::vector<fea::Drawable2D*> getDrawables();

    private:
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
};

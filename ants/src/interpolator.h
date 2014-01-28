#pragma once
#include <glm/glm.hpp>

class Interpolator
{
    public:
        Interpolator(const glm::vec2& initialPosition);
        void update();
        const glm::vec2& getPosition() const;
        void setPosition(const glm::vec2& position);
    private:
        glm::vec2 mCurrentPos;
        glm::vec2 mTargetPos;
};

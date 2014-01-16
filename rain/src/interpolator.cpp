#include "interpolator.h"

Interpolator::Interpolator(const glm::vec2& initialPosition) : mCurrentPos(initialPosition), mTargetPos(initialPosition)
{
}

void Interpolator::update()
{
    float distance = glm::distance(mTargetPos, mCurrentPos);
    if(distance < 2.0f)
        return;

    glm::vec2 direction = glm::normalize(mTargetPos - mCurrentPos);

    mCurrentPos += direction * distance / 4.0f;
}

const glm::vec2& Interpolator::getPosition() const
{
    return mCurrentPos;
}

void Interpolator::setPosition(const glm::vec2& position)
{
    mTargetPos = position;
}

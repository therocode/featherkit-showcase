#pragma once
#include "messages.h"

enum DiggingSpot { DIG_OFF = 0, DIG_A, DIG_B, DIG_C };

class Digging
    :   public fea::MessageReceiver<AntPositionMessage>,
        public fea::MessageReceiver<DiggerAntCreatedMessage>
{
    public:
        Digging(fea::MessageBus& bus);
        ~Digging();
        bool digging;

        void update();

        virtual void handleMessage(const AntPositionMessage& mess) override;
        virtual void handleMessage(const DiggerAntCreatedMessage& mess) override;

    private:
        glm::vec2 antPosition;
        size_t diggerAntId;

        glm::vec2 diggingPositionStartA;
        glm::vec2 diggingPositionEndA;
        glm::vec2 diggingPositionStartB;
        glm::vec2 diggingPositionEndB;

        int diggingPosition;

        float radius;

        fea::MessageBus& messageBus;
};

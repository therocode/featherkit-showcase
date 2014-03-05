#pragma once
#include "messages.h"

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

        fea::MessageBus& messageBus;
};

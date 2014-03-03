#pragma once
#include "messages.h"
#include "ant.h"

class AntManager
    :   public fea::MessageReceiver<AntOutsideBoundariesMessage>
{
    public:
        AntManager(fea::MessageBus& bus);
        ~AntManager();

        void update();
        size_t createAnt(bool type, bool goingRight, glm::vec2 position, float velocity);

        virtual void handleMessage(const AntOutsideBoundariesMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        std::unordered_map<size_t, Ant> ants;
        size_t nextAntId;

        void spawnAnts();

        glm::vec2 spawnPositionA;
        glm::vec2 spawnPositionB;
};

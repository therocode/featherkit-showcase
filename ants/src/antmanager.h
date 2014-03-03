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
        void createAnt(bool type, bool goingRight, glm::vec2 position);

        virtual void handleMessage(const AntOutsideBoundariesMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        std::unordered_map<size_t, Ant> ants;
        size_t nextAntId;

        void spawn();

        glm::vec2 spawnPositionA;
        glm::vec2 spawnPositionB;
};

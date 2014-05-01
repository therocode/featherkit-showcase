#pragma once
#include "messages.h"
#include "anttype.h"

class AntManager
    :   public fea::MessageReceiver<AntOutsideBoundariesMessage>
{
    public:
        AntManager(fea::MessageBus& bus);
        ~AntManager();

        void update();
        size_t createAnt(AntType type, bool goingRight, glm::vec2 position, float velocity);

        virtual void handleMessage(const AntOutsideBoundariesMessage& mess) override;

    private:
        fea::MessageBus& messageBus;
        size_t nextAntId;

        void spawnAnts();
        AntType randomCrystalAntType();

        glm::vec2 spawnPositionTop;
        glm::vec2 spawnPositionBottom;
};
